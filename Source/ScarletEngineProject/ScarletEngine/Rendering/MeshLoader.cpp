#include "ScarletEnginePch.h"
#include "MeshLoader.h"

#include <string>
#include <sstream>
#include <fstream>

#include <ScarletMath/Math.h>

#include "MeshData.h"

namespace Scarlet
{

namespace
{

struct Face
{
    uint32 index, normal, textureCoord;
};

struct VertexKey
{
    uint32 pos;
    uint32 uv;
    uint32 normal;

    bool operator==(const VertexKey& other) const
    {
        return pos    == other.pos    &&
               uv     == other.uv     &&
               normal == other.normal;
    }
};

struct VertexKeyHasher
{
    size_t operator()(const VertexKey& k) const
    {
        return ((k.pos    * 73856093) ^
                (k.uv     * 19349663) ^
                (k.normal * 83492791));
    }
};

enum class ObjKeyword : uint8
{
    ERR             = 0,
    VERTEX          = 1,
    FACE            = 2,
    OBJECT_NAME     = 3,
    TEXTURE_COORD   = 4,
    VERTEX_NORMAL   = 5,
    COMMENT         = 6,
    SMOOTHING_GROUP = 7
};

constexpr ObjKeyword StringToObjKeyword(const std::string_view& keyword)
{
    if (keyword == "v")  return ObjKeyword::VERTEX;
    if (keyword == "f")  return ObjKeyword::FACE;
    if (keyword == "o")  return ObjKeyword::OBJECT_NAME;
    if (keyword == "vt") return ObjKeyword::TEXTURE_COORD;
    if (keyword == "vn") return ObjKeyword::VERTEX_NORMAL;
    if (keyword == "#")  return ObjKeyword::COMMENT;
    if (keyword == "s")  return ObjKeyword::SMOOTHING_GROUP;

    return ObjKeyword::ERR;
}


bool FileEndsWithExtension(const char* str, const char* suffix)
{
    const size_t len = std::strlen(str);

    if (len < 4) return false;

    return std::strcmp(&str[len - 4], suffix) == 0;
}

} // Anonymous namespace.


void MeshLoader::LoadMesh(const char* filepath, Resource::MeshData& mesh)
{
    if (!FileEndsWithExtension(filepath, ".obj"))
    {
        SCARLET_WARN("Meshes have to be of type '.obj' extension, and hence failed to load given mesh at path: {}", filepath);
        return;
    }

    std::ifstream fin;
    fin.open(filepath, std::ios::in);

    if (fin.fail())
    {
        SCARLET_WARN("There was an error opening the mesh file at given path: {}", filepath);
        return;
    }

    vector<Face> faces;
    vector<Math::Vec3> normals;
    vector<Math::Vec2> textureCoordinates;

    std::string line;
    uint32 lineNumber = 0;

    while (std::getline(fin, line))
    {
        ++lineNumber;
        const size_t spacePosition = line.find(' ');
        if (spacePosition == std::string::npos)
        {
            SCARLET_WARN("Unrecognised line format when loading mesh at path: '{}'\nThe unrecognised line is: '{}: {}'", filepath, lineNumber, line);
            continue;
        }
        std::string         keywordString{ line.substr(0, spacePosition) };
        std::istringstream  input{ line.substr(spacePosition + 1) };


        switch (StringToObjKeyword(keywordString))
        {
        case ObjKeyword::VERTEX:
        {
            Math::Vec3 position;
            input >> position.x >> position.y >> position.z;
            mesh.vertices.emplace_back(position);
            break;
        }
        case ObjKeyword::FACE:
        {
            std::string faceStr[3];
            input >> faceStr[0] >> faceStr[2] >> faceStr[1];

            for (const auto& str : faceStr)
            {
                faces.emplace_back();

                const size_t slash1 = str.find('/');
                const size_t slash2 = str.find('/', slash1 + 1);

                /********************************************************************************************
                 * Example of obj export:
                 * ----------------------
                 *
                 * Case 1: [INVALID]
                 * -----------------
                 * 'f 2 3 1'
                 * This means the mesh was exported with only vertex data.
                 *
                 * Case 2: [INVALID]
                 * -----------------
                 * 'f 2//1 3//1 1//1'
                 * This means the mesh was exported with vertex and normal data.
                 *
                 * Case 3: [INVALID]
                 * -----------------
                 * 'f 2/1 3/2 1/3'
                 * This means that the mesh was exported with vertex and texture coordinates.
                 *
                 * Case 4: [VALID]
                 * ---------------
                 * 'f 2/1/1 3/2/1 1/3/1'
                 * This means the mesh was exported with vertex, normal and texture coordinates.
                 *
                 * ==========================================================================================
                 *
                 * Case 4 will be the only one that is handled due to the fact that the render pipeline
                 * needs texture coordinates and normals. Unexpected behaviour might occur if loaded in
                 * without all of the above. If work is done to renderer to add support, above cases will
                 * need to be implemented below.
                 ********************************************************************************************/

                // Invalid format if we start with a slash as this means there is no index data for the vertices.
                if (slash1 == std::string::npos || slash1 == 0 || slash2 == std::string::npos || slash1 + 1 == slash2)
                {
                    SCARLET_WARN("Failed to load mesh due to invalid face value for mesh at path: '{}'\nThe line with the invalid face is: '{}: {}'", filepath, lineNumber, line);
                    return;
                }

                faces.back().index = std::stoi(str.substr(0, slash1)) - 1;
                faces.back().textureCoord = std::stoi(str.substr(slash1 + 1, slash2 - slash1 - 1)) - 1;
                faces.back().normal = std::stoi(str.substr(slash2 + 1)) - 1;
            }
            break;
        }
        case ObjKeyword::TEXTURE_COORD:
        {
            Math::Vec2 textureCoord;
            input >> textureCoord.x >> textureCoord.y;
            textureCoordinates.emplace_back(textureCoord);
            break;
        }
        case ObjKeyword::VERTEX_NORMAL:
        {
            Math::Vec3 normal;
            input >> normal.x >> normal.y >> normal.z;
            normals.emplace_back(normal);
            break;
        }
        case ObjKeyword::COMMENT:
        case ObjKeyword::OBJECT_NAME:
        case ObjKeyword::SMOOTHING_GROUP:
            break;
        case ObjKeyword::ERR:
        {
            SCARLET_WARN("Failed to process line with the keyword '{0}' on line {1} when loading mesh at path: '{2}'.\n"                \
                         "Mesh will load with processed input, but could have unexpected results.", keywordString, lineNumber, filepath);
            break;
        }
        }
    }

    std::unordered_map<VertexKey, uint32, VertexKeyHasher> uniqueVertices;

    for (const Face& f : faces)
    {
        const VertexKey key{ .pos = f.index, .uv = f.textureCoord, .normal = f.normal };

        if (!uniqueVertices.contains(key))
        {
            Resource::Vertex v;
            v.position  = mesh.vertices[f.index].position;
            v.normal    = normals[f.normal];
            v.textureUv = textureCoordinates[f.textureCoord];

            mesh.vertices.emplace_back(v);
            uniqueVertices[key] = static_cast<uint32>(mesh.vertices.size()) - 1;
        }

        mesh.indices.push_back(uniqueVertices[key]);
    }
}

} // Namespace Scarlet.
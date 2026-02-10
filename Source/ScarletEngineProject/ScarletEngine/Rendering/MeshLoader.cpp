#include "ScarletEnginePch.h"
#include "MeshLoader.h"

#include <string>
#include <sstream>
#include <fstream>

namespace Scarlet
{

namespace
{

struct Face
{
    uint32 index, normal, textureCoord;
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
    if (keyword == "v")     return ObjKeyword::VERTEX;
    if (keyword == "f")     return ObjKeyword::FACE;
    if (keyword == "o")     return ObjKeyword::OBJECT_NAME;
    if (keyword == "vt")    return ObjKeyword::TEXTURE_COORD;
    if (keyword == "vn")    return ObjKeyword::VERTEX_NORMAL;
    if (keyword == "#")     return ObjKeyword::COMMENT;
    if (keyword == "s")     return ObjKeyword::SMOOTHING_GROUP;

    return ObjKeyword::ERR;
}

} // Anonymous namespace.

void MeshLoader::LoadMesh(const char* filepath, Resource::MeshData& mesh)
{
    //todo make debug or find a good way to check this
    if (filepath.GetExtension() != ".obj")
    {
        SCARLET_WARN("Meshes have to be of type '.obj' extension, and hence failed to load given mesh at path: {0}", filepath);
        // return error.
        return;
    }

    std::ifstream fin;
    fin.open(filepath, std::ios::in);

    if (fin.fail())
    {
        SCARLET_WARN("There was an error opening the mesh file at given path: {0}", filepath);
        // return error.
        return;
    }

    vector<Face> faces;
    vector<ScarlettMath::Vec3> normals;
    vector<ScarlettMath::Vec2> textureCoordinates;

    std::string line;
    uint32 lineNumber = 0;

    while (std::getline(fin, line))
    {
        ++lineNumber;
        const size_t spacePosition = line.find(' ');
        if (spacePosition == std::string::npos)
        {
            SCARLET_WARN("Unrecognised line format when loading mesh at path: '{0}'\nThe unrecognised line is: '{1}: {2}'", filepath, lineNumber, line);
            continue;
        }
        std::string         keywordString{ line.substr(0, spacePosition) };
        std::istringstream  input{ line.substr(spacePosition + 1) };


        switch (StringToObjKeyword(keywordString))
        {
        case ObjKeyword::VERTEX:
        {
            ScarlettMath::Vec3 position;
            input >> position.x >> position.y >> position.z;
            mesh.vertices.emplace_back(position);
            break;
        }
        case ObjKeyword::FACE:
        {
            std::string faceStr[3];
            input >> faceStr[0] >> faceStr[1] >> faceStr[2];

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
                    // return error.
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
            ScarlettMath::Vec2 textureCoord;
            input >> textureCoord.x >> textureCoord.y;
            textureCoordinates.emplace_back(textureCoord);
            break;
        }
        case ObjKeyword::VERTEX_NORMAL:
        {
            ScarlettMath::Vec3 normal;
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

    for (const Face& f : faces)
    {
        if (normals.size() <= f.normal)
        {
            SCARLET_WARN("Failed to load mesh due to a mismatch between the number of normals and the normal index for the mesh with path: '{0}'", filepath);
            // return error.
            return;
        }

        if (textureCoordinates.size() <= f.textureCoord)
        {
            SCARLET_WARN("Failed to load mesh due to a mismatch between the number of texture coordinates and the texture coordinates index for the mesh with path: '{0}'", filepath);
            // return error.
            return;
        }

        if (mesh.vertices.size() <= f.index)
        {
            SCARLET_WARN("Failed to load mesh due to a mismatch between the number of vertices and the index for the mesh with path: '{0}'", filepath);
            // return error.
            return;
        }

        mesh.indices.emplace_back(f.index);

        auto& [_, normal, textureCoord] = mesh.vertices[f.index];
        normal = normals[f.normal];
        textureCoord = textureCoordinates[f.textureCoord];
    }
}

} // Namespace Scarlet.
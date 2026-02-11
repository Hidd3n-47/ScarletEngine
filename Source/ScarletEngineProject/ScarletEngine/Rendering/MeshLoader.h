#pragma once

namespace Scarlet
{

namespace Resource
{
struct MeshData;
} // Namespace Scarlet::Resource.

/**
* @class MeshLoader: A static class used to load in meshes from a given filepath.
*/
class MeshLoader
{
public:
    /*
     * @brief Load a mesh from the provided filepath.
     * @note: Only 'obj' mesh types are supported.
     * @param filepath: The relative filepath of the asset.
     * @param mesh: The output of the loaded mesh.
     */
    static void LoadMesh(const char* filepath, Resource::MeshData& mesh);
};

} // Namespace Scarlet.
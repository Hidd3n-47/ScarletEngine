#pragma once

#include "Vertex.h"

namespace Scarlet::Resource
{
/**
 * @struct MeshData: The data loaded in from disk for a given mesh. This mesh data is used to construct a \ref Mesh.
 */
struct MeshData
{
    vector<Vertex> vertices;
    vector<uint32> indices;
};

} // Namespace Scarlet::Resource.

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

    DEBUG(Math::Vec3 lowerBound{ std::numeric_limits<float>::max() });
    DEBUG(Math::Vec3 upperBound{ std::numeric_limits<float>::min() });
};

} // Namespace Scarlet::Resource.

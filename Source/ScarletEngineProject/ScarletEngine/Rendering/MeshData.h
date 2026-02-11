#pragma once

#include "Vertex.h"

namespace Scarlet::Resource
{

struct MeshData
{
    vector<Vertex> vertices;
    vector<uint32> indices;
};

} // Namespace Scarlet::Resource.

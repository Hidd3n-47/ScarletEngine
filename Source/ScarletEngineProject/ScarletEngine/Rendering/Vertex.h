#pragma once

#include <ScarletMath/Math.h>

namespace Scarlet::Resource
{

struct Vertex
{
    Math::Vec3 position;
    Math::Vec3 normal;
    Math::Vec2 textureCoord;
};

} // Namespace Scarlet::Resource.
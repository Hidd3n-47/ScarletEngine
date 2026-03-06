#pragma once

#include <ScarletMath/Math.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct Transform
{
    Math::Vec3 position;
    Math::Vec3 scale;

    SCARLET_COMPONENT(Transform);
};

} // Namespace Scarlet::Component.
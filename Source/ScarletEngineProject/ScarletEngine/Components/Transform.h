#pragma once

#include <ScarletMath/Quat.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct Transform
{
    Math::Vec3 translation;
    Math::Quat rotation;
    Math::Vec3 scale;

    SCARLET_COMPONENT(Transform);
};

} // Namespace Scarlet::Component.
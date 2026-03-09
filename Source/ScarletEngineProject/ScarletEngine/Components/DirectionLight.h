#pragma once

#include <ScarletMath/Math.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct DirectionLight
{
    Math::Vec3 lightColor  = { 1.0f, 1.0f, 1.0f };
    Math::Vec3 direction   = { -1.0f, 1.0f, -1.0f };
    float ambientIntensity = 0.190f;
    float diffuseIntensity = 0.960f;
    float rimPower         = 3.4f;

    SCARLET_COMPONENT(DirectionLight)
};

} // Namespace Scarlet::Component.
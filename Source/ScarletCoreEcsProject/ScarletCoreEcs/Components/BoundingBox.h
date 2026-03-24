#pragma once

#include <ScarletMath/Math.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct BoundingBox
{
    Math::Vec3 localMinimum{ -0.5f, -0.5f, -0.5f };
    Math::Vec3 localMaximum{  0.5f,  0.5f,  0.5f };

    inline Math::Vec3 GetCenter() const { return (localMinimum + localMaximum) * 0.5f; }

    SCARLET_COMPONENT(BoundingBox)
};

} // Namespace Scarlet::Component.
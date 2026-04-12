#pragma once

#include <functional>

#include <ScarlEnt/RTTI/Rtti.h>

#include <ScarletCore/PrimitiveTypes.h>

namespace Scarlet::Component
{

struct SphereCollider
{
    float  radius = 0.5f;
    uint32 layer  = 0;

    std::function<void(SphereCollider&)> onCollisionCallback;

    SCARLET_COMPONENT(SphereCollider)
};

} // Namespace Scarlet::Component.
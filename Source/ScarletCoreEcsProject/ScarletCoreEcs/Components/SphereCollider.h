#pragma once

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct SphereCollider
{
    float radius = 0.5f;

    SCARLET_COMPONENT(SphereCollider)
};

} // Namespace Scarlet::Component.
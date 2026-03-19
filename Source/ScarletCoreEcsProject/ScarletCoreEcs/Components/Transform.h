#pragma once

#include "ScarletCoreEcs/Defines.h"

#include <ScarletMath/Quat.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct COMPONENT_API Transform
{
    Math::Vec3 translation{ };
    Math::Vec3 rotation   { };
    Math::Vec3 scale      { 1.0f };

    SCARLET_COMPONENT(Transform);
};

} // Namespace Scarlet::Component.
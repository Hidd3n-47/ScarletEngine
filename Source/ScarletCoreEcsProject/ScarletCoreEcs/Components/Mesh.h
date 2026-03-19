#pragma once

#include "ScarletCoreEcs/Defines.h"

#include <ScarlEnt/RTTI/Rtti.h>

#include <ScarletCore/AssetRef.h>

namespace Scarlet::Component
{

struct COMPONENT_API Mesh
{
    AssetRef mesh     = { AssetType::MESH    , 0 };
    AssetRef material = { AssetType::MATERIAL, 0 };

    SCARLET_COMPONENT(Mesh)
};

} // Namespace Scarlet::Component.
 
#pragma once

#include <ScarlEnt/RTTI/Rtti.h>

#include <ScarletCore/AssetRef.h>

namespace Scarlet::Component
{

struct Mesh
{
    AssetRef mesh     = { AssetType::MESH    , 0 };
    AssetRef material = { AssetType::MATERIAL, 0 };

    SCARLET_COMPONENT(Mesh)
};

} // Namespace Scarlet::Component.
 
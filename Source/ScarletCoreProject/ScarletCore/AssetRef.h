#pragma once

#include "ScarletCore/PrimitiveTypes.h"

namespace Scarlet
{

    // todo christian Comment this.
enum class AssetType : uint8
{
    MESH     = 0,
    TEXTURE  = 1,
    MATERIAL = 2,

    ASSET_TYPE_COUNT
};

struct AssetRef
{
    AssetType assetType;
    uint64    assetUlid;
};

} // Namespace Scarlet
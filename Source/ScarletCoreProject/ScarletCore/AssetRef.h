#pragma once

#include "ScarletCore/PrimitiveTypes.h"

namespace Scarlet
{

enum class AssetType : uint8
{
    MESH     = 0,
    TEXTURE  = 1,
    MATERIAL = 2,

    ASSET_TYPE_COUNT
};

struct AssetRef
{
    AssetRef() = default;
    AssetRef(const uint32 typeAsInt, const uint64 ulid) : assetType(static_cast<AssetType>(typeAsInt)), assetUlid(ulid) {}
    AssetRef(const AssetType type  , const uint64 ulid) : assetType(type), assetUlid(ulid) { }
    AssetType assetType;
    uint64    assetUlid;
};

} // Namespace Scarlet
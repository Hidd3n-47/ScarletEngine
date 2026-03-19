#include "ScarletEnginePch.h"
#include "ILazyLoadAsset.h"

#include "Core/Engine.h"
#include "AssetLoading/AssetManager.h"

namespace Scarlet::Resource
{

ILazyLoadAsset::ILazyLoadAsset(const uint32 assetTypeAsInt, const Ulid ulid)
{
    *this = *Engine::Instance().GetAssetManager().GetAsset(static_cast<AssetType>(assetTypeAsInt), ulid).GetRawPtr();
}

} //Namespace Scarlet::Resource.
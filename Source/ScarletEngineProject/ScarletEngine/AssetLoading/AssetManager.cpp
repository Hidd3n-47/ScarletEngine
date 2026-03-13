#include "ScarletEnginePch.h"
#include "AssetManager.h"

namespace Scarlet
{

AssetManager::~AssetManager()
{
    for (const Resource::ILazyLoadAsset* asset : mAssets)
    {
        delete asset;
    }
}

void AssetManager::RemoveAsset(WeakHandle<Resource::ILazyLoadAsset>& asset)
{
    mAssets.erase(asset.GetRawPtr());
    delete asset.GetRawPtr();

    asset.Invalidate();
}

} // Namespace Scarlet.
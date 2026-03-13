#pragma once

#include "LazyLoadAsset.h"

namespace Scarlet
{

/**
 * @class AssetManager: A class responsible for the creation and destruction of assets. This class is purley here to control lifetime of assets.
 */
class AssetManager
{
public:
    AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&)            = delete;
    AssetManager(AssetManager&&)                 = delete;
    AssetManager& operator=(AssetManager&&)      = delete;
    AssetManager& operator=(const AssetManager&) =  delete;

    /**
     * @brief Create an asset at the given filepath.
     * @tparam Asset The type of asset being created.
     * @param filepath The filepath the asset is on disk.
     * @return A \ref WeakHandle to \ref ILazyLoadAsset for the created asset.
     */
    template <typename Asset>
    WeakHandle<Resource::ILazyLoadAsset> CreateAsset(std::string filepath);

    /**
     * @brief Destroy and remove a created asset.
     * @param asset The asset being destroyed and removed.
     */
    void RemoveAsset(WeakHandle<Resource::ILazyLoadAsset>& asset);
private:
    unordered_set<Resource::ILazyLoadAsset*> mAssets;
};

/* ============================================================================================================================== */

template <typename Asset>
WeakHandle<Resource::ILazyLoadAsset> AssetManager::CreateAsset(std::string filepath)
{
    Resource::ILazyLoadAsset* asset = new Resource::LazyLoadAsset<Asset>(std::move(filepath));
    mAssets.emplace(asset);
    return WeakHandle{ asset };
}

} // Namespace Scarlet.

#pragma once

#include "ScarletEngine/Core/Defines.h"

#include <array>

#include "LazyLoadAsset.h"

namespace Scarlet
{

/**
 * @class AssetManager: A class responsible for the creation and destruction of assets. This class is purley here to control lifetime of assets.
 */
class ENGINE_API AssetManager
{
public:
    AssetManager() = default;
    ~AssetManager();

    AssetManager(const AssetManager&)            = delete;
    AssetManager(AssetManager&&)                 = delete;
    AssetManager& operator=(AssetManager&&)      = delete;
    AssetManager& operator=(const AssetManager&) = delete;

    /**
     * @brief Iterate through the assets directory and load in the scarlet assets.
     * @param assetPath: The starting path to directory iterate through, loading in any scarlet assets found.
     */
    void LoadScarletAssets(const Filepath& assetPath);

    /**
     * @brief Get an asset based off the \ref AssetType and unique identifier.
     * @param type The \ref AssetType.
     * @param ulid The unique identifier of the asset.
     * @return A \ref WeakHandle to \ref ILazyLoadAsset of the asset with the given type and unique identifier.
     */
    WeakHandle<Resource::ILazyLoadAsset> GetAsset(const AssetType type, const Ulid ulid) { return WeakHandle{ mTypeArrayForUlidToAssets[static_cast<int>(type)][ulid] }; }

    /**
     * @brief Create an asset at the given filepath.
     * @tparam Asset The type of asset being created.
     * @param type: The \ref AssetType of the asset.
     * @param filepath The \ref Filepath the asset is on disk.
     * @return A \ref WeakHandle to \ref ILazyLoadAsset for the created asset.
     */
    template <typename Asset>
    WeakHandle<Resource::ILazyLoadAsset> CreateAsset(const AssetType type, const Filepath& filepath, const Ulid ulid = {});

    /**
     * @brief Destroy and remove a created asset.
     * @param asset The asset being destroyed and removed.
     */
    void RemoveAsset(WeakHandle<Resource::ILazyLoadAsset>& asset);

    /**
     * @brief Get all the assets of a specific type. This will return all assets created from @see \c AssetManager::LoadScarletAssets.
     * @note The assets are created, but might not be loaded due to being \ref ILazyLoadAsset.
     * @param type The \ref AssetType.
     * @return An unordered map between unique identifier and the \ref ILazyLoadAsset for the given \ref AssetType.
     */
    inline const unordered_map<Ulid, Resource::ILazyLoadAsset*>& GetLoadedAssets(const AssetType type) const { return mTypeArrayForUlidToAssets[static_cast<int>(type)]; }

    static constexpr Ulid INVALID_ULID{ 0 };
private:
    std::array<unordered_map<Ulid, Resource::ILazyLoadAsset*>, static_cast<int>(AssetType::ASSET_TYPE_COUNT)> mTypeArrayForUlidToAssets;
};

/* ============================================================================================================================== */

template <typename Asset>
WeakHandle<Resource::ILazyLoadAsset> AssetManager::CreateAsset(const AssetType type, const Filepath& filepath, const Ulid ulid /* = {} */)
{
    Resource::ILazyLoadAsset* asset = new Resource::LazyLoadAsset<Asset>(type, ulid, std::move(filepath));
    mTypeArrayForUlidToAssets[static_cast<int>(type)][ulid] = asset;
    return WeakHandle{ asset };
}

} // Namespace Scarlet.

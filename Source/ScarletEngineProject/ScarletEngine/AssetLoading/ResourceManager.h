#pragma once

namespace Scarlet
{

namespace Resource
{
class Texture;
template <typename T>
class LazyLoadAsset;
} // Namespace Resource.

/**
 * @class ResourceManager: A class responsible for loading a particular type of resource. This class is different from the \ref AssetManager.
 * This class is responsible for loading an asset from disk and freeing up the resources.
 * The asset pipeline:
 * \ref AssetManager  -> Returns a \ref ILazyLoadAsset. This is purely for controlling the lifetime of lazy assets for its lifetime.
 * \ref LazyLoadAsset -> Handles lazily loading in the asset.The \ref ResourceManager for the \c Asset will be called to load in the actual asset from disk.
 * @tparam Asset The type of asset.
 */
template <typename Asset>
class ResourceManager
{
    friend class Renderer;
public:
    ~ResourceManager()
    {
        for (size_t i{ 0 }; i < mLoadedAsset.size(); ++i)
        {
            delete mLoadedAsset[i];
        }
    }

    /**
     * @brief Load in the asset from disk.
     * @param lazyAsset The \ref LazyLoadAsset for the asset. This is stored for when other assets are removed,
     * meaning the runtime identifier will be changed due to using a vector. This is preferred over using maps to prevent the lookup cost.
     * @param filepath The filepath of the asset.
     */
    void Load(Resource::LazyLoadAsset<Asset>* lazyAsset, const Filepath& filepath)
    {
        if (mAssetPathToLoadedAssetIndex.contains(filepath))
        {
            // Already loaded, no op needed.
            return;
        }

        lazyAsset->SetRuntimeId(static_cast<uint32>(mLoadedAsset.size()));

        mLoadedAsset.emplace_back(new Asset{ filepath });
        mLazyLoadAsset.emplace_back(lazyAsset);

        mAssetPathToLoadedAssetIndex[filepath] = lazyAsset->GetRuntimeId();
    }

    /**
     * @brief Internal function to get a \ref WeakHandle to the resource.
     * @param runtimeIndex The runtime index into the loaded assets dynamic array.
     * @return A \ref WeakHandle to the asset at the requested runtime index.
     */
    WeakHandle<Asset> GetResource(const uint32 runtimeIndex) const
    {
        return WeakHandle{ mLoadedAsset[runtimeIndex] };
    }

private:
    vector<Asset*> mLoadedAsset;
    vector<Resource::LazyLoadAsset<Asset>*> mLazyLoadAsset;
    unordered_map<Filepath, size_t>         mAssetPathToLoadedAssetIndex;
};

} // Namespace Scarlet.
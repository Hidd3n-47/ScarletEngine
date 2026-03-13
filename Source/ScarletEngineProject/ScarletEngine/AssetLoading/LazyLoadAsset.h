#pragma once

#include "ILazyLoadAsset.h"

#include "Rendering/Renderer.h"

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

namespace Scarlet::Resource
{

/**
 * @class LazyLoadAsset: A class representing the \ref ILazyLoadAsset with its type.
 * @see LazyLoadAsset
 * @tparam Asset The type of the asset.
 */
template <typename Asset>
class LazyLoadAsset final : public ILazyLoadAsset
{
public:
    LazyLoadAsset(const std::string& filepath) : ILazyLoadAsset(std::move(filepath)) { }

    /**
     * @brief Set the runtime identifier of the asset. This should only be used by the managers (\ref ResourceManager) when loading the asset.
     * @param runtimeId The runtime identifier of the asset.
     */
    inline void SetRuntimeId(const uint32 runtimeId) { mRuntimeId = runtimeId; }

    /**
     * @brief Load in the specific type of asset.
     */
    void LoadAsset() override;
};

/* ============================================================================================================================== */

template <typename Asset>
inline void LazyLoadAsset<Asset>::LoadAsset()
{
    static_assert(false && "Failed to load asset, add loading function for type.");
}

template<>
inline void LazyLoadAsset<Mesh>::LoadAsset()
{
    SCARLET_ASSERT(mRuntimeId == INVALID_ID && "Trying to load in the asset multiple times.");

    Renderer::Instance().GetMeshManager().Load(this, mAssetPath);
}

template<>
inline void LazyLoadAsset<Texture>::LoadAsset()
{
    SCARLET_ASSERT(mRuntimeId == INVALID_ID && "Trying to load in the asset multiple times.");

    Renderer::Instance().GetTextureManager().Load(this, mAssetPath);
}

} // Namespace Scarlet::Resource.
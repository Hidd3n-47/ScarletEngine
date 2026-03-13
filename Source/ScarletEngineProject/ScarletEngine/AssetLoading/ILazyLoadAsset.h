#pragma once

#include "ScarletCore/Ulid.h"

namespace Scarlet::Resource
{

/**
 * @class ILazyLoadAsset: An 'interface' used to abstract the type of resource out, but can still interact to get unique and runtime ID's.
 * The asset that this represents will be loaded in 'lazily' meaning it will only be loaded when it is used.
 */
class ILazyLoadAsset
{
public:
    ILazyLoadAsset(std::string asset) : mAssetPath(std::move(asset)){ }
    virtual ~ILazyLoadAsset() = default;

    /**
     * @brief Get the unique \ref Ulid of the asset.
     * @return The unique \ref Ulid of the asset.
     */
    [[nodiscard]] uint32 GetUlid() const { return mUlid; }
    /**
     * @brief Get the runtime identifier of the asset. If the asset is not loaded in, this will load the asset in.
     * @remark If the asset has not been loaded when calling this method, it will load in the asset before returning the runtime identifier.
     * @return The runtime identifier of the asset.
     */
    [[nodiscard]] uint32 GetRuntimeId() { if (mRuntimeId == INVALID_ID) LoadAsset(); return mRuntimeId; }

    /**
     * @brief Load in the asset.
     */
    virtual void LoadAsset() = 0;

    inline static constexpr uint32 INVALID_ID{ std::numeric_limits<uint32>::max() };
protected:
    Ulid        mUlid{};
    uint32      mRuntimeId{ INVALID_ID };
    std::string mAssetPath;

};

} // Namespace Scarlet::Resource.

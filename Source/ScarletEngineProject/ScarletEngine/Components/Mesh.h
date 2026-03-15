#pragma once

#include <ScarlEnt/RTTI/Rtti.h>

#include "ScarletEngine/Core/Engine.h"
#include "ScarletEngine/AssetLoading/AssetManager.h"
#include "ScarletEngine/AssetLoading/LazyLoadAsset.h"

namespace Scarlet::Component
{

struct Mesh
{
    WeakHandle<Resource::ILazyLoadAsset> mesh     = Engine::Instance().GetAssetManager().GetAsset(AssetType::MESH, AssetManager::INVALID_ULID);
    WeakHandle<Resource::ILazyLoadAsset> material = Engine::Instance().GetAssetManager().GetAsset(AssetType::MATERIAL, AssetManager::INVALID_ULID);

    SCARLET_COMPONENT(Mesh)
};

} // Namespace Scarlet::Component.

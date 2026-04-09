#include "Scene.h"

#include <ranges>

namespace ScarlEnt
{

Scene::Scene(const std::string_view friendlyName)
    : mFriendlyName(friendlyName)
{
    // Empty.
}

#ifdef DEV_CONFIGURATION
Scene::~Scene()
{
    for (const IEntityHandle* handle : mEntityIdToHandleMap | std::views::values)
    {
        delete handle;
    }
}

void Scene::RemoveMutableHandle(const IEntityHandle* entity)
{
    for (auto it = mMutableEntityHandles.begin(); it != mMutableEntityHandles.end(); ++it)
    {
        if (*it == entity)
        {
            mMutableEntityHandles.erase(it);
            return;
        }
    }
}
#endif // DEV_CONFIGURATION.

MutableEntityHandle Scene::AddMutableEntity()
{
#ifdef DEV_CONFIGURATION
    auto handle = MutableEntityHandle{ Scarlet::WeakHandle{ this }, Scarlet::WeakHandle{ &mComponentManager } };
#else // ^^^ DEV_CONFIGURATION ^^^ | vvv !DEV_CONFIGURATION vvv
    auto handle = MutableEntityHandle{ Scarlet::WeakHandle{ &mComponentManager } };
#endif // ^^^ !DEV_CONFIGURATION ^^^

    const Scarlet::Ulid entityId{ handle.GetEntityId().uniqueId };

    mEntityIdToHandleMap[entityId] = new MutableEntityHandle(handle);

#ifdef DEV_CONFIGURATION
    mMutableEntityHandles.emplace_back(mEntityIdToHandleMap[entityId]);
#endif // ^^^ DEV_CONFIGURATION ^^^

    return handle;
}

} // Namespace ScarlEnt.
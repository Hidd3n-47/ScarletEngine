#include "Scene.h"

namespace ScarlEnt
{

Scene::Scene(const std::string_view friendlyName, const size_t index)
    : mFriendlyName(friendlyName)
    , mRegistryIndex(index)
{
    // Empty.
}

void Scene::Update()
{
    mComponentManager.Update();
}

MutableEntityHandle Scene::AddMutableEntity()
{
    return MutableEntityHandle{ Scarlet::WeakHandle{ &mComponentManager } };
}

} // Namespace ScarlEnt.
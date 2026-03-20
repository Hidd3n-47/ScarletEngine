#include "Scene.h"

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
    for (const IEntityHandle* handle : mEntityHandles)
    {
        delete handle;
    }
}
#endif // DEV_CONFIGURATION.

MutableEntityHandle Scene::AddMutableEntity()
{
#ifdef DEV_CONFIGURATION
    auto handle = MutableEntityHandle{ Scarlet::WeakHandle{ &mComponentManager } };
    mMutableEntityHandles.emplace_back(new MutableEntityHandle(handle));
    return handle;
#else // DEV_CONFIGURATION.
    return MutableEntityHandle{ Scarlet::WeakHandle{ &mComponentManager } };
#endif // !DEV_CONFIGURATION
}

} // Namespace ScarlEnt.
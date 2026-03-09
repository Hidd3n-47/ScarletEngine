#include "Registry.h"

#include "Scene.h"

namespace ScarlEnt
{

void Registry::Init()
{
    SCARLENT_ASSERT(!mInstance && "Instance of Registry already exists, no need to re-initialise.");

    mInstance = new Registry();
}

void Registry::Terminate()
{
    delete mInstance;
    mInstance = nullptr;
}

Scarlet::WeakHandle<Scene> Registry::CreateScene(const std::string_view friendlyName)
{
    Scene* scene = new Scene{ friendlyName };
    mScenes[std::string{ friendlyName }] = scene;

    return Scarlet::WeakHandle{ scene };
}

void Registry::RemoveScene(Scarlet::WeakHandle<Scene>& scene)
{
    SCARLENT_ASSERT(scene.IsValid());

    const std::string friendlyName{ scene->GetFriendlyName() };

    SCARLENT_ASSERT(mScenes.contains(friendlyName) && "Trying to remove a scene that isn't in the scene map.");

    delete mScenes[friendlyName];
    mScenes.erase(friendlyName);

    scene.Invalidate();
}

void Registry::SetActiveScene(const Scarlet::WeakHandle<Scene> scene)
{
    SCARLENT_ASSERT(scene.IsValid());

    mActiveScene = scene;
}

#ifdef DEV_CONFIGURATION
void Registry::AddComponentToHandle(const char* componentType, IEntityHandle* handle)
{
    SCARLENT_ASSERT(mComponentToAddComponentFunction.contains(componentType) && "Trying to add a component to an entity that hasn't been registered.");

    mComponentToAddComponentFunction[componentType](handle);
}
#endif // DEV_CONFIGURATION.

} // Namespace ScarlEnt.
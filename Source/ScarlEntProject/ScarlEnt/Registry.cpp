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

void Registry::PostUpdate()
{
    if (mSceneToChangeTo.IsValid())
    {
        Scarlet::WeakHandle<Scene> currentScene = mActiveScene;

        mActiveScene = mSceneToChangeTo;

        if (mDestroySceneOnChange)
        {
            RemoveScene(currentScene);
        }
    }

    mSceneToChangeTo.Invalidate();
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

    if (mActiveScene == scene)
    {
        mActiveScene.Invalidate();
    }

    const std::string friendlyName{ scene->GetFriendlyName() };

    SCARLENT_ASSERT(mScenes.contains(friendlyName) && "Trying to remove a scene that isn't in the scene map.");

    delete mScenes[friendlyName];
    mScenes.erase(friendlyName);

    scene.Invalidate();
}

void Registry::ChangeScene(const Scarlet::WeakHandle<Scene> newScene, const bool destroySceneOnChange)
{
    SCARLENT_ASSERT(!mSceneToChangeTo.IsValid() && "Already trying to change scene this frame.");

    mSceneToChangeTo      = newScene;
    mDestroySceneOnChange = destroySceneOnChange;
}

void Registry::SetActiveScene(const Scarlet::WeakHandle<Scene> scene)
{
    SCARLENT_ASSERT(!mActiveScene.IsValid() && "Trying to change when scene is active. Use Registry::ChangeScene to safely change scene at runtime.");

    mActiveScene = scene;
}

#ifdef DEV_CONFIGURATION
unordered_map<std::string, Property>* Registry::AddComponentToHandle(const char* componentType, IEntityHandle* handle)
{
    SCARLENT_ASSERT(mComponentToAddComponentFunction.contains(componentType) && "Trying to add a component to an entity that hasn't been registered.");

    return mComponentToAddComponentFunction[componentType](handle);
}
#endif // DEV_CONFIGURATION.

} // Namespace ScarlEnt.
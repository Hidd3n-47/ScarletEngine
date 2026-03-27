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

#ifndef DEV_CONFIGURATION
        mActiveScene->Init();
#endif // !DEV_CONFIGURATION.

        if (mDestroySceneOnChange)
        {
            RemoveScene(currentScene);
        }
    }

    mSceneToChangeTo.Invalidate();
}

void Registry::RemoveScene(const std::string& friendlyName)
{
    SCARLENT_ASSERT(mScenes.contains(friendlyName) && "Trying to remove a scene with a friendlyName that hasn't been added.");

    Scarlet::WeakHandle scene = GetScene(friendlyName);
    if (scene.IsValid()) [[likely]]
    {
        RemoveScene(scene);
    }
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

#ifndef DEV_CONFIGURATION
    scene->Destroy();
#endif // !DEV_CONFIGURATION.

    delete mScenes[friendlyName];
    mScenes.erase(friendlyName);

    scene.Invalidate();
}

void Registry::RenameScene(Scarlet::WeakHandle<Scene> scene, const std::string& newName)
{
    const std::string currentName = std::string{ scene->GetFriendlyName() };
    Scene* currentScene = mScenes[currentName];

    mScenes.erase(currentName);

    mScenes[newName] = currentScene;
    currentScene->mFriendlyName = newName;
}

Scarlet::WeakHandle<Scene> Registry::GetScene(const std::string& friendlyName) const
{
    if (!mScenes.contains(friendlyName))
    {
        return Scarlet::WeakHandle<Scene>{ };
    }

    return Scarlet::WeakHandle{ mScenes.at(friendlyName) };
}

void Registry::ChangeScene(const std::string& sceneToChangeToFriendlyName, const bool destroySceneOnChange)
{
    SCARLENT_ASSERT(mScenes.contains(sceneToChangeToFriendlyName) && "Trying to retrieve a scene by friendly name that hasn't been added.");

    ChangeScene(Scarlet::WeakHandle{ mScenes[sceneToChangeToFriendlyName] }, destroySceneOnChange);
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

#ifndef DEV_CONFIGURATION
    mActiveScene->Init();
#endif // !DEV_CONFIGURATION.
}

#ifdef DEV_CONFIGURATION
PropertyMap* Registry::AddComponentToHandle(const char* componentType, IEntityHandle* handle)
{
    SCARLENT_ASSERT(mComponentToTypelessFunctionMap.contains(componentType) && "Trying to add a component to an entity that hasn't been registered.");

    return mComponentToTypelessFunctionMap[componentType].addComponent(handle);
}

bool Registry::HasHandleGotComponent(const char* componentType, IEntityHandle* handle)
{
    SCARLENT_ASSERT(mComponentToTypelessFunctionMap.contains(componentType) && "Trying to add a component to an entity that hasn't been registered.");

    return mComponentToTypelessFunctionMap[componentType].hasComponent(handle);
}

void Registry::RemoveComponentFromHandle(const char* componentType, IEntityHandle* handle)
{
    SCARLENT_ASSERT(mComponentToTypelessFunctionMap.contains(componentType) && "Trying to add a component to an entity that hasn't been registered.");

    mComponentToTypelessFunctionMap[componentType].removeComponent(handle);
}
#endif // DEV_CONFIGURATION.

} // Namespace ScarlEnt.

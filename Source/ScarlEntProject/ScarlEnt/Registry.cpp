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
}

Scarlet::WeakHandle<Scene> Registry::CreateScene(const std::string_view friendlyName)
{
    mScenes.emplace_back(new Scene{ friendlyName, mScenes.size() });

    return Scarlet::WeakHandle{ mScenes.back() };
}

void Registry::RemoveScene(Scarlet::WeakHandle<Scene>& scene)
{
    SCARLENT_ASSERT(scene.IsValid());

    const size_t index = scene->GetRegistryIndex();

    delete mScenes[index];

    for (size_t i{ index + 1 }; i < mScenes.size(); ++i)
    {
        --mScenes[i]->mRegistryIndex;
    }

    mScenes.erase(mScenes.begin() + static_cast<long long>(index));

    scene.Invalidate();
}

void Registry::SetActiveScene(const Scarlet::WeakHandle<Scene> scene)
{
    SCARLENT_ASSERT(scene.IsValid());

    mCurrentSceneIndex = static_cast<int32>(scene->GetRegistryIndex());
}

} // Namespace ScarlEnt.
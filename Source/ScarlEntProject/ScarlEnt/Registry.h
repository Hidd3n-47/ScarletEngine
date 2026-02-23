#pragma once

#include <string>

#include <ScarletCore/vector.h>
#include <ScarletCore/unordered_map.h>

#include <ScarletCore/WeakHandle.h>
#include <ScarletCore/PrimitiveTypes.h>

#include "Defines.h"

namespace ScarlEnt
{

class Scene;

/**
 * @class Registry: A singleton class used to deal with the ECS. The registry should be initialised before use and then used to interface with the ECS.
 */
class SCARLENT_API Registry
{
public:
    Registry(const Registry&)            = delete;
    Registry(Registry&&)                 = delete;
    Registry& operator=(Registry&&)      = delete;
    Registry& operator=(const Registry&) = delete;

    [[nodiscard]] inline static Registry& Instance() { return *mInstance; }

    /**
     * @brief Create and initialise the Registry for the ECS.
     */
    static void Init();
    /**
     * @brief Terminate and free the Registry used for the ECS.
     */
    static void Terminate();

    /**
     * @brief Create a new \ref Scene.
     * @param friendlyName The friendly name of the scene.
     * @return A \ref WeakHandle to the created scene.
     */
    [[nodiscard]] Scarlet::WeakHandle<Scene> CreateScene(const std::string_view friendlyName);
    /**
     * @brief Remove a scene.
     * @param scene The \ref WeakHandle to a scene that is to be destroyed.
     */
    void RemoveScene(Scarlet::WeakHandle<Scene>& scene);

    /**
     * @brief Set the currently active scene.
     * @param scene The scene that will be set to the active scene.
     */
    void SetActiveScene(const Scarlet::WeakHandle<Scene> scene);

    /**
     * @brief Register or get a component bit. This function is to dynamically allocate bits \\n
     * at runtime without having to manually map a bit identifier to a component. This will retrieve the bit \\n
     * if previously registered, or create a new bit if not previously registered.
     * @remark Current implementation returns a 64-bit integer, meaning the max number of registered components cannot exceed 64 currently.
     * @tparam T The component class or structure.
     * @return The unique bit of the component.
     */
    template <typename T>
    uint64 GetOrRegisterComponentBit();

    /**
     * @brief Get the currently active scene.
     * @return Returns a \ref WeakHandle to the currently active scene. This scene will be invalid if there is no currently active scene.
     */
    [[nodiscard]] inline Scarlet::WeakHandle<Scene> GetActiveScene() const 
                        { return mCurrentSceneIndex >=0 ? Scarlet::WeakHandle{ mScenes[mCurrentSceneIndex] } : Scarlet::WeakHandle<Scene>{}; }

    /**
     * @brief Get the ID for the component type.
     * @tparam ComponentType The type of the component.
     * @return The type ID for the component.
     */
    template <typename ComponentType>
    [[nodiscard]] inline static const char* GetComponentTypeId() { return typeid(ComponentType).name(); }

#ifdef SCARLENT_TEST
    /**
     * @brief Get the number of scenes created.
     * @remark Only available in test solution.
     * @return The number of scenes created.
     */
    [[nodiscard]] inline size_t GetNumberOfScenes() const { return mScenes.size(); }
#endif // SCARLENT_TEST.
private:
    Registry()  = default;
    ~Registry() = default;

    inline static Registry* mInstance = nullptr;

    unordered_map<const char*, uint64> mComponentIdMap;
    uint32 mNumberOfRegisteredComponents = 0;

    int32 mCurrentSceneIndex = -1;
    vector<Scene*> mScenes;
};

/* ============================================================================================================================== */

template <typename T>
inline uint64 Registry::GetOrRegisterComponentBit()
{
    // Map a component to its TypeId name.
    const char* componentName = GetComponentTypeId<T>();

    if (mComponentIdMap.contains(componentName))
    {
        return mComponentIdMap[componentName];
    }

    SCARLENT_ASSERT(mNumberOfRegisteredComponents < 63 && "Currently the registry can only handle 64 components. Limits reached.");

    const uint64 componentId = 1 << mNumberOfRegisteredComponents++;
    mComponentIdMap[componentName] = componentId;

    return componentId;
}

} // Namespace ScarlEnt
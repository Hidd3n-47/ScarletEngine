#pragma once

#include <string>

#include <ScarletCore/vector.h>
#include <ScarletCore/unordered_map.h>

#include <ScarletCore/WeakHandle.h>
#include <ScarletCore/PrimitiveTypes.h>

#include "Defines.h"
#include "ComponentId.h"

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
    [[nodiscard]] ComponentId GetOrRegisterComponentId();

    /**
     * @brief Get the identifier of the component from the bitmask.
     * @param bitmask The bitmask of the component.
     * @return The identifier of the component.
     */
    [[nodiscard]] uint32 GetComponentIdFromBitmask(const uint64 bitmask);

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

    static constexpr size_t COMPONENTS_PAGE_SIZE = 10;
    static constexpr size_t COMPONENT_BITSET_PAGE_SIZE = 1000;
private:
    Registry()  = default;
    ~Registry() = default;

    inline static Registry* mInstance = nullptr;

    unordered_map<const char*, ComponentId> mComponentIdMap;
    unordered_map<uint64, uint32> mComponentBitmaskToId;
    uint32 mNumberOfRegisteredComponents = 0;

    int32 mCurrentSceneIndex = -1;
    vector<Scene*> mScenes;
};

/* ============================================================================================================================== */

template <typename T>
inline ComponentId Registry::GetOrRegisterComponentId()
{
    // Map a component to its TypeId name.
    const char* componentName = GetComponentTypeId<T>();

    if (mComponentIdMap.contains(componentName))
    {
        return mComponentIdMap[componentName];
    }

    SCARLENT_ASSERT(mNumberOfRegisteredComponents < 63 && "Currently the registry can only handle 64 components. Limits reached.");

    const uint64 bitmask = 1ull << mNumberOfRegisteredComponents;
    mComponentIdMap[componentName] = ComponentId{ .id = mNumberOfRegisteredComponents, .bitmask = bitmask };
    mComponentBitmaskToId[bitmask] = mNumberOfRegisteredComponents;

    ++mNumberOfRegisteredComponents;

    return mComponentIdMap[componentName];
}

inline uint32 Registry::GetComponentIdFromBitmask(const uint64 bitmask)
{
    SCARLENT_ASSERT(mComponentBitmaskToId.contains(bitmask) && "Trying to get component ID for a non-registered component");
    return mComponentBitmaskToId[bitmask];
}


} // Namespace ScarlEnt
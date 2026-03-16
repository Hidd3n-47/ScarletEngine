#pragma once

#include <functional>

#include <ScarletCore/vector.h>
#include <ScarletCore/unordered_map.h>

#include <ScarletCore/WeakHandle.h>
#include <ScarletCore/PrimitiveTypes.h>

#include "Defines.h"
#include "ComponentId.h"

#ifdef DEV_CONFIGURATION
#include "RTTI/Property.h"
#endif // DEV_CONFIGURATION.

namespace ScarlEnt
{

class Scene;
class IEntityHandle;

/**
 * @class Registry: A singleton class used to deal with the ECS. The registry should be initialised before use and then used to interface with the ECS.
 */
class SCARLENT_API Registry
{
public:
    DEBUG(using AddComponentFunc = std::function<unordered_map<std::string, Property>* (IEntityHandle*)>);

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
     * @brief Used to perform operations after the updates have completed, this ensures safe removal of entities/scenes as no system or entity is currently in use/referenced whist destroying.
     */
    void PostUpdate();

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
     * @brief A function used to change from an active scene to a new scene. This is the safe way to prevent issues with executing systems and lifetime.
     * @param newScene The new scene that will become active once all systems have been changed.
     * @param destroySceneOnChange \c true to destroy the scene after successfully changing the active scene to the \c newScene, \c false otherwise.
     */
    void ChangeScene(const Scarlet::WeakHandle<Scene> newScene, const bool destroySceneOnChange = false);

    /**
     * @brief Set the currently active scene.
     * @remark This is unsafe to do in the middle of scene updates as the active entities or systems of the scene could be referenced. At runtime rather use \ref Registry::ChangeScene.
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
    [[nodiscard]] inline Scarlet::WeakHandle<Scene> GetActiveScene() const { return mActiveScene; }

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

#ifdef DEV_CONFIGURATION
    void RegisterComponent(const std::string& componentName, const AddComponentFunc& addComponentFunction) { mComponentToAddComponentFunction[componentName] = addComponentFunction; }
    unordered_map<std::string, Property>* AddComponentToHandle(const char* componentType, IEntityHandle* handle);
    [[nodiscard]] inline const unordered_map<std::string, AddComponentFunc>& GetComponentToAddComponentMap() const { return mComponentToAddComponentFunction; }
#endif // DEV_CONFIGURATION.

    static constexpr size_t COMPONENTS_PAGE_SIZE = 10;
    static constexpr size_t COMPONENT_BITSET_PAGE_SIZE = 1000;
private:
    Registry()  = default;
    ~Registry() = default;

    inline static Registry* mInstance = nullptr;

    Scarlet::WeakHandle<Scene> mSceneToChangeTo;
    bool mDestroySceneOnChange;

    DEBUG(unordered_map<std::string, AddComponentFunc> mComponentToAddComponentFunction);

    unordered_map<std::string, ComponentId> mComponentIdMap;
    unordered_map<uint64, uint32> mComponentBitmaskToId;
    uint32 mNumberOfRegisteredComponents = 0;

    Scarlet::WeakHandle<Scene> mActiveScene;
    unordered_map<std::string, Scene*> mScenes;
};

/* ============================================================================================================================== */

template <typename T>
inline ComponentId Registry::GetOrRegisterComponentId()
{
    // Map a component to its TypeId name.
    const std::string typeId       = GetComponentTypeId<T>();
    const size_t lastColonPosition = typeId.find_last_of(':');

    const std::string componentName = typeId.substr(lastColonPosition + 1, typeId.length() - lastColonPosition - 1);

    if (mComponentIdMap.contains(componentName))
    {
        return mComponentIdMap[componentName];
    }

    SCARLENT_ASSERT(mNumberOfRegisteredComponents < 63 && "Currently the registry can only handle 64 components. Limits reached.");

    const uint64 bitmask = 1ull << mNumberOfRegisteredComponents;

#ifdef DEV_CONFIGURATION
    mComponentIdMap[componentName] = ComponentId{ .id = mNumberOfRegisteredComponents, .bitmask = bitmask, .name = componentName };
#else // DEV_CONFIGURATION.
    mComponentIdMap[componentName] = ComponentId{ .id = mNumberOfRegisteredComponents, .bitmask = bitmask };
#endif // !DEV_CONFIGURATION.

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
#pragma once

#include <ScarletCore/Ulid.h>

#include "EntityHandle.h"
#include "MutableEntityHandle.h"

#include "ComponentManager.h"

namespace ScarlEnt
{
/**
 * @class Scene: A class to hold a collection of entities, components and handles system updates.
 */
class SCARLENT_API Scene
{
    friend class Registry;
public:
    virtual void Init()    { }
    virtual void Destroy() { }

    /**
     * @brief Update the scene. This will update all the registered systems.
     */
    inline void Update() { mComponentManager.Update(); }

    /**
     * @brief Update the scene at a fixed timestep set in \ref Time.
     */
    inline void FixedUpdate() { mComponentManager.FixedUpdate(); }

    /**
     * @brief Register a system that will act over a subset of components calling the update function.
     * @tparam Components The components the system acts on.
     * @param updateFunction The function acting on the components.
     */
    template <typename...Components>
    void RegisterSystem(const std::function<void(Components&...)>& updateFunction);

    /**
     * @brief Register a system that will act over a subset of components calling the update function in the fixed update.
     * @tparam Components The components the system acts on.
     * @param updateFunction The function acting on the components.
     */
    template <typename...Components>
    void RegisterFixedUpdateSystem(const std::function<void(Components&...)>& updateFunction);

    /**
     * @brief Remove all the registered systems.
     */
    inline void RemoveAllSystems() { mComponentManager.RemoveAllSystems(); }

    /**
     * @brief Add an entity to the scene.
     * @tparam ArchetypeComponents The component types of the archetype.
     * @return A \ref EntityHandle to the entity.
     */
    template <typename...ArchetypeComponents>
    [[nodiscard]] EntityHandle<ArchetypeComponents...> AddEntity();

    /**
     * @brief Add an entity to the scene.
     * @tparam ArchetypeComponents The component types of the archetype.
     * @tparam Args The argument types of the component constructors.
     * @param args The forwarded arguments passed to the constructors of the components.
     * @return A \ref EntityHandle to the entity.
     */
    template <typename...ArchetypeComponents, typename...Args>
    [[nodiscard]] EntityHandle<ArchetypeComponents...> AddEntity(Args&& ...args);

    /**
     * @brief Create and add a mutable entity to the scene.
     * @return A \ref MutableEntityHandle to the entity.
     */
    [[nodiscard]] MutableEntityHandle AddMutableEntity();

    /**
     * @brief Remove an entity from the scene.
     * @tparam ArchetypeComponents The component types of the archetype.
     * @param entity A \ref EntityHandle to the entity.
     */
    template <typename...ArchetypeComponents>
    void RemoveEntity(const EntityHandle<ArchetypeComponents...> entity);

    /** @brief Get the friendly name of the scene */
    [[nodiscard]] inline std::string_view GetFriendlyName() const { return mFriendlyName; }

    /**
     * @brief Set the handle to the camera entity. Each scene needs this to allow for the renderer to show through the main camera.
     * @param handle The handle to the immutable entity that contains the camera, transform and direction light.
     */
    inline void SetCameraEntityHandle(IEntityHandle* handle) { mCameraEntity = handle; }
    /**
     * @brief Get the handle to the camera entity. The entity can be cast to the correct format to be used by the renderer.
     * @remark The entity is immutable and contains the camera, transform and direction light.
     * @return A pointer to the handle of the entity that contains the camera, transform and direction light used in the rendering.
     */
    [[nodiscard]] inline IEntityHandle* GetCameraEntityHandle() const { return mCameraEntity; }

#ifdef DEV_CONFIGURATION
    [[nodiscard]] inline const vector<IEntityHandle*>& GetEntityHandles()        const { return mEntityHandles; }
    [[nodiscard]] inline const vector<IEntityHandle*>& GetMutableEntityHandles() const { return mMutableEntityHandles; }

    void RemoveMutableHandle(const IEntityHandle* entity);
#endif // DEV_CONFIGURATION.
protected:
    Scene(const std::string_view friendlyName);
#ifdef DEV_CONFIGURATION
    virtual ~Scene();
#else // DEV_CONFIGURATION.
    virtual ~Scene() = default;
#endif // !DEV_CONFIGURATION.

private:
    std::string mFriendlyName;

    ComponentManager mComponentManager;

    IEntityHandle* mCameraEntity = nullptr;

    DEBUG(vector<IEntityHandle*> mEntityHandles;)
    DEBUG(vector<IEntityHandle*> mMutableEntityHandles;)
};

/* ============================================================================================================================== */

template <typename...Components>
inline void Scene::RegisterSystem(const std::function<void(Components&...)>& updateFunction)
{
    mComponentManager.RegisterSystem<Components...>(updateFunction);
}

template <typename...Components>
inline void Scene::RegisterFixedUpdateSystem(const std::function<void(Components&...)>& updateFunction)
{
    mComponentManager.RegisterFixedUpdateSystem<Components...>(updateFunction);
}

template <typename... ArchetypeComponents>
inline EntityHandle<ArchetypeComponents...> Scene::AddEntity()
{
    return AddEntity<ArchetypeComponents...>(ArchetypeComponents{}...);
}

template <typename... ArchetypeComponents, typename... Args>
inline EntityHandle<ArchetypeComponents...> Scene::AddEntity(Args&&... args)
{
#ifdef DEV_CONFIGURATION
    auto handle = EntityHandle<ArchetypeComponents...>{ Scarlet::WeakHandle{ &mComponentManager }, std::forward<Args>(args)... };
    mEntityHandles.emplace_back(new EntityHandle<ArchetypeComponents...>(handle));
    return handle;
#else // DEV_CONFIGURATION.
    return EntityHandle<ArchetypeComponents...>{ Scarlet::WeakHandle{ &mComponentManager }, std::forward<Args>(args)... };
#endif // !DEV_CONFIGURATION
}

template <typename...ArchetypeComponents>
void Scene::RemoveEntity(const EntityHandle<ArchetypeComponents...> entity)
{
    mComponentManager.RemoveEntity<ArchetypeComponents...>(entity.mEntityId);
}

} // Namespace ScarlEnt.

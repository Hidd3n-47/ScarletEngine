#pragma once

#include <ScarletCore/Ulid.h>

#include "EntityHandle.h"
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
    /**
     * @brief Update the scene. This will update all the registered systems.
     */
    void Update() const;

    /**
     * @brief Register a system that will act over a subset of components calling the update function.
     * @tparam Components The components the system acts on.
     * @param updateFunction The function acting on the components.
     */
    template <typename...Components>
    void RegisterSystem(const std::function<void(Components&...)>& updateFunction);

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
     * @brief Remove an entity from the scene.
     * @tparam ArchetypeComponents The component types of the archetype.
     * @param entity A \ref EntityHandle to the entity.
     */
    template <typename...ArchetypeComponents>
    void RemoveEntity(const EntityHandle<ArchetypeComponents...> entity);

    /** @brief Get the friendly name of the scene */
    [[nodiscard]] inline std::string_view GetFriendlyName() const { return mFriendlyName; }

    /** @brief Get the index of the scene in the registry. */
    [[nodiscard]] inline size_t GetRegistryIndex() const { return mRegistryIndex; }
private:
    Scene(const std::string_view friendlyName, const size_t index);

    std::string mFriendlyName;
    size_t      mRegistryIndex;

    ComponentManager mComponentManager;
};

/* ============================================================================================================================== */

inline void Scene::Update() const
{
    mComponentManager.Update();
}

template <typename...Components>
inline void Scene::RegisterSystem(const std::function<void(Components&...)>& updateFunction)
{
    mComponentManager.RegisterSystem<Components...>(updateFunction);
}

template <typename... ArchetypeComponents, typename... Args>
inline EntityHandle<ArchetypeComponents...> Scene::AddEntity(Args&&... args)
{
    return EntityHandle<ArchetypeComponents...>{ Scarlet::WeakHandle{ &mComponentManager }, std::forward<Args>(args)... };
}

template <typename...ArchetypeComponents>
void Scene::RemoveEntity(const EntityHandle<ArchetypeComponents...> entity)
{
    mComponentManager.RemoveEntity<ArchetypeComponents...>(entity.mEntityId);
}

} // Namespace ScarlEnt.
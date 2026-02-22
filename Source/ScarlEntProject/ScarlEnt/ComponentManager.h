#pragma once

#include <string>
#include <functional>

#include <ScarletCore/Ulid.h>

#include "Registry.h"
#include "Archetype.h"

namespace ScarlEnt
{

typedef std::string ComponentTypeId;

/**
 * @class ComponentManager: The owner of the components and instance used to create and destroy components.<br/>
 */
class ComponentManager
{
public:
    ComponentManager() = default;
    ~ComponentManager();

    ComponentManager(const ComponentManager&)            = delete;
    ComponentManager(ComponentManager&&)                 = delete;
    ComponentManager& operator=(ComponentManager&&)      = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

    /**
     * @brief Add an entity to an Archetype defined by the components from the templated arguments.
     * @tparam ArchetypeComponents The components that create the archetype.
     * @tparam Args The argument types for the components constructors.
     * @param args The forward arguments to the constructors of the components.
     * @return The \ref Ulid of the entity.
     */
    template <typename...ArchetypeComponents, typename...Args>
    [[nodiscard]] Scarlet::Ulid AddEntity(Args&&...args);

    /**
     * @brief Remove an entity from an archetype.
     * @tparam ArchetypeComponents The components of the archetype.
     * @param entityId The identifier to the entity that is being removed.
     */
    template <typename...ArchetypeComponents>
    void RemoveEntity(Scarlet::Ulid entityId);

    /**
     * @brief Get a reference to a component on an entity.
     * @tparam Component The component being requested.
     * @tparam ArchetypeComponents The components of the entity, this is to retrieve the component from the correct archetype.
     * @param entityId The identifier to the entity whose component is being requested for.
     * @return A reference to the component on the entity.
     */
    template <typename Component, typename...ArchetypeComponents>
    [[nodiscard]] Component& GetComponent(const Scarlet::Ulid entityId);

    /**
     * @brief Get the ID for the component type.
     * @tparam ComponentType The type of the component.
     * @return The type ID for the component.
     */
    template <typename ComponentType>
    inline static ComponentTypeId GetComponentTypeId() { return ComponentTypeId{ typeid(ComponentType).name() }; }

private:
    unordered_map<uint64, IComponentArray*> mComponents;
    unordered_map<Scarlet::Ulid, IComponentArray*> mEntityIdToComponentArray;
};

/* ============================================================================================================================== */

template <typename... ArchetypeComponents, typename... Args>
inline Scarlet::Ulid ComponentManager::AddEntity(Args&&... args)
{
    const Scarlet::Ulid entityId{};

    const uint64 componentsBitset = (Registry::Instance().GetOrRegisterComponentBit<ArchetypeComponents>() | ...);

    if (!mComponents.contains(componentsBitset))
    {
        mComponents[componentsBitset] = new Archetype<ArchetypeComponents...>();
    }

    mEntityIdToComponentArray[entityId] = mComponents[componentsBitset];

    static_cast<Archetype<ArchetypeComponents...>*>(mComponents[componentsBitset])->AddEntity(entityId, std::forward<Args>(args)...);
    return entityId;
}

template <typename... ArchetypeComponents>
inline void ComponentManager::RemoveEntity(Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToComponentArray.contains(entityId) && "Trying to remove an entity that has not been created.");

    IComponentArray* array = mEntityIdToComponentArray[entityId];
    mEntityIdToComponentArray.erase(entityId);

    static_cast<Archetype<ArchetypeComponents...>*>(array)->RemoveEntity(entityId);
}

template <typename Component, typename...ArchetypeComponents>
inline Component& ComponentManager::GetComponent(const Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToComponentArray.contains(entityId) && "Trying to retrieve a component that has not been created.");

    IComponentArray* array = mEntityIdToComponentArray[entityId];

    return static_cast<Archetype<ArchetypeComponents...>*>(array)->template GetComponent<Component>(entityId);
}

} // Namespace ScarlEnt.
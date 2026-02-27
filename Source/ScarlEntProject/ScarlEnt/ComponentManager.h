#pragma once

#include <string>
#include <functional>

#include <ScarletCore/Ulid.h>

#include "System.h"
#include "Registry.h"
#include "Archetype.h"
#include "SparseSet.h"
#include "MutableEntityId.h"

namespace ScarlEnt
{

typedef std::string ComponentTypeId;

/**
 * @class ComponentManager: The owner of the components and instance used to create and destroy components.<br/>
 */
class SCARLENT_API ComponentManager
{
public:
    template <typename Component>
    using MutableComponentArray = SparseSet<Component, Registry::COMPONENT_BITSET_PAGE_SIZE>;

    ComponentManager() = default;
    ~ComponentManager();

    ComponentManager(const ComponentManager&)            = delete;
    ComponentManager(ComponentManager&&)                 = delete;
    ComponentManager& operator=(ComponentManager&&)      = delete;
    ComponentManager& operator=(const ComponentManager&) = delete;

    /**
     * @brief Register a system that will act over a subset of components calling the update function.
     * @tparam Components The components the system acts on.
     * @param updateFunction A function pointer to the function acting on the components.
     */
    template <typename...Components>
    void RegisterSystem(const std::function<void(Components&...)>& updateFunction)
    {
        mSystems.emplace_back(new System<Components...>(updateFunction));
    }

    /**
     * @brief Go through each registered system and update. Each archetype will be iterated over and compared to the required components of the system, updating the components \\n
     * based on the registered update function if so.
     */
    void Update();

    /**
     * @brief Add an entity to an Archetype defined by the components from the templated arguments.
     * @tparam ArchetypeComponents The components that create the archetype.
     * @param values The forward arguments to the constructors of the components.
     * @return The \ref Ulid of the entity.
     */
    template <typename...ArchetypeComponents>
    [[nodiscard]] Scarlet::Ulid AddEntity(ArchetypeComponents&&...values);

    /**
     * @brief Create and add mutable entity.
     * @return A \ref MutableEntityId of the mutable entity that was added.
     */
    [[nodiscard]] MutableEntityId AddMutableEntity();

    /**
     * @brief Remove a mutable entity. This will remove all the components associated with the mutable entity.
     * @param mutableEntityRuntimeId: The runtime identifier of the mutable entity that is being removed.
     */
    void RemoveMutableEntity(const uint32 mutableEntityRuntimeId);

    /**
     * @brief Add a component to a mutable entity.
     * @tparam Component The component being added to the mutable entity.
     * @param mutableEntityRuntimeId The runtime identifier of the mutable entity whose component is being added.
     */
    template <typename Component, typename...Args>
    void AddComponent(const uint32 mutableEntityRuntimeId, Args&&...args);

    /**
     * @brief Remove a component from a mutable entity.
     * @tparam Component The component being removed from the mutable entity.
     * @param mutableEntityRuntimeId The runtime identifier of the mutable entity whose component is being removed.
     */
    template <typename Component>
    void RemoveComponent(const uint32 mutableEntityRuntimeId);

    /**
     * @brief Get a reference to a component on a mutable entity.
     * @tparam Component The component being requested.
     * @param mutableEntityRuntimeId The runtime identifier of the mutable entity whose component is being requested.
     * @return A reference to the component on the mutable entity.
     */
    template <typename Component>
    [[nodiscard]] Component& GetMutableEntityComponent(const uint32 mutableEntityRuntimeId);

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

#ifdef SCARLENT_TEST
    [[nodiscard]] inline uint64 GetMutableEntityComponentBitset(const uint32 runtimeMutableId) { return mMutableEntityToComponentBitset[runtimeMutableId]; }
    [[nodiscard]] inline const SparseSet<ISparseComponentArray*, Registry::COMPONENTS_PAGE_SIZE>& GetMutableComponentArrays() const { return mComponentIdToSparseSetArray; }
#endif // SCARLENT_TEST

private:
    // Archetype - i.e. Immutable entities.
    unordered_map<uint64, Archetype*>        mArchetypeComponents;
    unordered_map<Scarlet::Ulid, Archetype*> mEntityIdToArchetypeComponentArray;

    // SparseSet - i.e. Mutable entities.
    uint32 mMutableEntitiesId = 0;
    vector<ISparseComponentArray*> mSparseSetComponentArrays;
    SparseSet<ISparseComponentArray*, Registry::COMPONENTS_PAGE_SIZE> mComponentIdToSparseSetArray;
    SparseSet<uint64, Registry::COMPONENT_BITSET_PAGE_SIZE>           mMutableEntityToComponentBitset;
    unordered_map<uint64, std::unordered_set<uint32>>                 mComponentBitsetToMutableEntities;

    // Systems.
    vector<ISystem*> mSystems;
};

/* ============================================================================================================================== */

template <typename... ArchetypeComponents>
inline Scarlet::Ulid ComponentManager::AddEntity(ArchetypeComponents&&... values)
{
    const Scarlet::Ulid entityId{};

    const uint64 componentsBitset = (Registry::Instance().GetOrRegisterComponentId<ArchetypeComponents>().bitmask | ...);

    if (!mArchetypeComponents.contains(componentsBitset))
    {
        mArchetypeComponents[componentsBitset] = new Archetype(entityId, std::forward<ArchetypeComponents>(values)...);
        mEntityIdToArchetypeComponentArray[entityId] = mArchetypeComponents[componentsBitset];
        return entityId;
    }
    mEntityIdToArchetypeComponentArray[entityId] = mArchetypeComponents[componentsBitset];

    mArchetypeComponents[componentsBitset]->AddEntity(entityId, std::forward<ArchetypeComponents>(values)...);
    return entityId;
}

template <typename Component, typename...Args>
inline void ComponentManager::AddComponent(const uint32 mutableEntityRuntimeId, Args&&...args)
{
    const ComponentId componentId = Registry::Instance().GetOrRegisterComponentId<Component>();

    if (!mComponentIdToSparseSetArray.Contains(componentId.id))
    {
        mSparseSetComponentArrays.emplace_back(new MutableComponentArray<Component>());
        mComponentIdToSparseSetArray.Add(componentId.id, mSparseSetComponentArrays.back());
    }

    const uint64 entityBitset = mMutableEntityToComponentBitset.Pop(mutableEntityRuntimeId);
    const uint64 newBitset = entityBitset | componentId.bitmask;

    mMutableEntityToComponentBitset.Add(mutableEntityRuntimeId, newBitset);

    mComponentBitsetToMutableEntities[entityBitset].erase(mutableEntityRuntimeId);
    mComponentBitsetToMutableEntities[newBitset].emplace(mutableEntityRuntimeId);

    static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id])->Add(mutableEntityRuntimeId, std::forward<Args>(args)...);
}

template <typename Component>
inline void ComponentManager::RemoveComponent(const uint32 mutableEntityRuntimeId)
{
    const ComponentId componentId = Registry::Instance().GetOrRegisterComponentId<Component>();

    SCARLENT_ASSERT(mComponentIdToSparseSetArray.Contains(componentId.id) && "Trying to remove a component that hasn't been added.");

    const uint64 entityBitset = mMutableEntityToComponentBitset.Pop(mutableEntityRuntimeId);
    const uint64 newBitset = entityBitset & ~componentId.bitmask;

    if (!mMutableEntityToComponentBitset.Contains(mutableEntityRuntimeId))
    {
        mMutableEntityToComponentBitset.Add(mutableEntityRuntimeId, newBitset);
    }

    mMutableEntityToComponentBitset[mutableEntityRuntimeId] = newBitset;

    mComponentBitsetToMutableEntities[entityBitset].erase(mutableEntityRuntimeId);
    mComponentBitsetToMutableEntities[newBitset].emplace(mutableEntityRuntimeId);

    SCARLENT_ASSERT(static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id])->Contains(mutableEntityRuntimeId) && "Trying to remove a component that hasn't been added to entity.");
    static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id])->Remove(mutableEntityRuntimeId);
}

template <typename Component>
inline Component& ComponentManager::GetMutableEntityComponent(const uint32 mutableEntityRuntimeId)
{
    const ComponentId componentId = Registry::Instance().GetOrRegisterComponentId<Component>();
    SCARLENT_ASSERT(mComponentIdToSparseSetArray.Contains(componentId.id) && "Trying to get a component that hasn't been added to an entity yet.");

    return (*static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id]))[mutableEntityRuntimeId];
}

template <typename... ArchetypeComponents>
inline void ComponentManager::RemoveEntity(Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToArchetypeComponentArray.contains(entityId) && "Trying to remove an entity that has not been created.");

    Archetype* array = mEntityIdToArchetypeComponentArray[entityId];
    mEntityIdToArchetypeComponentArray.erase(entityId);

    array->RemoveEntity(entityId);
}

template <typename Component, typename...ArchetypeComponents>
inline Component& ComponentManager::GetComponent(const Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToArchetypeComponentArray.contains(entityId) && "Trying to retrieve a component that has not been created.");

    Archetype* array = mEntityIdToArchetypeComponentArray[entityId];

    return array->GetComponent<Component>(entityId);
}

} // Namespace ScarlEnt.
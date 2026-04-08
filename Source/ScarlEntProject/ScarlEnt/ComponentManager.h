#pragma once

#include <string>
#include <functional>

#include <ScarletCore/Ulid.h>

#include "System.h"
#include "Registry.h"
#include "Archetype.h"
#include "SparseSet.h"
#include "MutableEntityId.h"
#include "RTTI/ComponentView.h"

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
    void RegisterSystem(const std::function<void(Components&...)>& updateFunction);

    /**
     * @brief Register a system that will act over a subset of components calling the update function in the fixed update.
     * @tparam Components The components the system acts on.
     * @param updateFunction A function pointer to the function acting on the components.
     */
    template <typename...Components>
    void RegisterFixedUpdateSystem(const std::function<void(Components&...)>& updateFunction);

    /**
     * @brief Remove all the registered systems - fixed as well as normal systems.
     */
    void RemoveAllSystems();

    /**
     * @brief Go through each registered system and update. Each archetype will be iterated over and compared to the required components of the system, updating the components \\n
     * based on the registered update function if so.
     */
    void Update();

    /**
     * @brief Post update is called after all the systems have been iterated through, allowing for safe removal of components.
     */
    void PostUpdate();

    /**
     * @brief Go through each registered system and update at a fixed frame rate. Each archetype will be iterated over and compared to the required components of the system, \\n
     * updating the components based on the registered update function if so.
     */
    void FixedUpdate();

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
    Component& AddComponent(const uint32 mutableEntityRuntimeId, Args&&...args);

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

    /**
     * @brief Get all subset of components that have all the requested components.
     * @tparam Components The subset of components that an entity has to have to be included in this filter.
     * @remark This is auto return type so that the returned vector uses move semantics instead of copied each time.
     * @return A vector of a tuples (tuple of component references of those components requested).
     */
    template <typename...Components>
    [[nodiscard]] auto GetComponents();

#ifdef SCARLENT_TEST
    [[nodiscard]] inline uint64 GetMutableEntityComponentBitset(const uint32 runtimeMutableId) { return mMutableEntityToComponentBitset[runtimeMutableId]; }
    [[nodiscard]] inline const SparseSet<ISparseComponentArray*, Registry::COMPONENTS_PAGE_SIZE>& GetMutableComponentArrays() const { return mComponentIdToSparseSetArray; }
#endif // SCARLENT_TEST.

#ifdef DEV_CONFIGURATION
    [[nodiscard]] inline const vector<ComponentView>& GetEntityComponentView(const Scarlet::Ulid entityId) { return mEntityIdToComponentViews[entityId]; }
    [[nodiscard]] inline const vector<ComponentView>& GetMutableEntityComponentView(const uint32 entityId) { return mMutableEntityIdToComponentViews[entityId]; }
#endif // DEV_CONFIGURATION.

private:
    // Archetype - i.e. Immutable entities.
    unordered_map<uint64, Archetype*>        mArchetypeComponents;
    unordered_map<Scarlet::Ulid, Archetype*> mEntityIdToArchetypeComponentArray;
#ifdef DEV_CONFIGURATION
    unordered_map<Scarlet::Ulid, vector<ComponentView>> mEntityIdToComponentViews;
#endif // DEV_CONFIGURATION.

    // SparseSet - i.e. Mutable entities.
    uint32 mMutableEntitiesId = 0;
    vector<ISparseComponentArray*> mSparseSetComponentArrays;
    SparseSet<ISparseComponentArray*, Registry::COMPONENTS_PAGE_SIZE> mComponentIdToSparseSetArray;
    SparseSet<uint64, Registry::COMPONENT_BITSET_PAGE_SIZE>           mMutableEntityToComponentBitset;
    unordered_map<uint64, std::unordered_set<uint32>>                 mComponentBitsetToMutableEntities;
#ifdef DEV_CONFIGURATION
    unordered_map<uint32, vector<ComponentView>>                      mMutableEntityIdToComponentViews;
#endif // DEV_CONFIGURATION.

    // Systems.
    vector<ISystem*> mSystems;
    vector<ISystem*> mFixedSystems;

    vector<std::function<void()>> mFunctionsToExecutePostUpdate;
};

/* ============================================================================================================================== */

template <typename...Components>
void ComponentManager::RegisterSystem(const std::function<void(Components&...)>& updateFunction)
{
    mSystems.emplace_back(new System<Components...>(updateFunction));
}

template <typename...Components>
void ComponentManager::RegisterFixedUpdateSystem(const std::function<void(Components&...)>& updateFunction)
{
    mFixedSystems.emplace_back(new System<Components...>(updateFunction));
}

template <typename... ArchetypeComponents>
inline Scarlet::Ulid ComponentManager::AddEntity(ArchetypeComponents&&... values)
{
    const Scarlet::Ulid entityId{};

    const uint64 componentsBitset = (Registry::Instance().GetOrRegisterComponentId<ArchetypeComponents>().bitmask | ...);

    if (!mArchetypeComponents.contains(componentsBitset))
    {
        mArchetypeComponents[componentsBitset] = new Archetype(entityId, std::forward<ArchetypeComponents>(values)...);
        mEntityIdToArchetypeComponentArray[entityId] = mArchetypeComponents[componentsBitset];

#ifdef DEV_CONFIGURATION
        (mEntityIdToComponentViews[entityId].emplace_back(
            Registry::Instance().GetOrRegisterComponentId<ArchetypeComponents>(),
            [&, componentsBitset, entityId] { return mArchetypeComponents[componentsBitset]->GetComponent<ArchetypeComponents>(entityId).GetProperties(); }), ...);
#endif // DEV_CONFIGURATION.

        return entityId;
    }
    mEntityIdToArchetypeComponentArray[entityId] = mArchetypeComponents[componentsBitset];

    mArchetypeComponents[componentsBitset]->AddEntity(entityId, std::forward<ArchetypeComponents>(values)...);

#ifdef DEV_CONFIGURATION
    (mEntityIdToComponentViews[entityId].emplace_back(
        Registry::Instance().GetOrRegisterComponentId<ArchetypeComponents>(),
        [&, componentsBitset, entityId] { return mArchetypeComponents[componentsBitset]->GetComponent<ArchetypeComponents>(entityId).GetProperties(); }), ...);
#endif // DEV_CONFIGURATION.

    return entityId;
}

template <typename Component, typename...Args>
inline Component& ComponentManager::AddComponent(const uint32 mutableEntityRuntimeId, Args&&...args)
{
    const ComponentId componentId = Registry::Instance().GetOrRegisterComponentId<Component>();

    if (!mComponentIdToSparseSetArray.Contains(componentId.id))
    {
        mSparseSetComponentArrays.emplace_back(new MutableComponentArray<Component>());
        mComponentIdToSparseSetArray.Add(componentId.id, mSparseSetComponentArrays.back());
    }

    const uint64 entityBitset = mMutableEntityToComponentBitset.Pop(mutableEntityRuntimeId);
    const uint64 newBitset    = entityBitset | componentId.bitmask;

    mMutableEntityToComponentBitset.Add(mutableEntityRuntimeId, newBitset);

    mComponentBitsetToMutableEntities[entityBitset].erase(mutableEntityRuntimeId);
    mComponentBitsetToMutableEntities[newBitset].emplace(mutableEntityRuntimeId);

    Component& comp = static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id])->Add(mutableEntityRuntimeId, std::forward<Args>(args)...);

#ifdef DEV_CONFIGURATION
    mMutableEntityIdToComponentViews[mutableEntityRuntimeId].emplace_back(componentId, [&, mutableEntityRuntimeId] { return GetMutableEntityComponent<Component>(mutableEntityRuntimeId).GetProperties(); });
#endif // DEV_CONFIGURATION.

    return comp;
}

template <typename Component>
inline void ComponentManager::RemoveComponent(const uint32 mutableEntityRuntimeId)
{
    mFunctionsToExecutePostUpdate.emplace_back([&, mutableEntityRuntimeId]
    {
        const ComponentId componentId = Registry::Instance().GetOrRegisterComponentId<Component>();

        SCARLENT_ASSERT(mComponentIdToSparseSetArray.Contains(componentId.id) && "Trying to remove a component that hasn't been added.");

        const uint64 entityBitset = mMutableEntityToComponentBitset.Pop(mutableEntityRuntimeId);
        const uint64 newBitset    = entityBitset & ~componentId.bitmask;

        if (!mMutableEntityToComponentBitset.Contains(mutableEntityRuntimeId))
        {
            mMutableEntityToComponentBitset.Add(mutableEntityRuntimeId, newBitset);
        }

        mMutableEntityToComponentBitset[mutableEntityRuntimeId] = newBitset;

        mComponentBitsetToMutableEntities[entityBitset].erase(mutableEntityRuntimeId);
        mComponentBitsetToMutableEntities[newBitset].emplace(mutableEntityRuntimeId);

        SCARLENT_ASSERT(static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id])->Contains(mutableEntityRuntimeId) && "Trying to remove a component that hasn't been added to entity.");
        static_cast<MutableComponentArray<Component>*>(mComponentIdToSparseSetArray[componentId.id])->Remove(mutableEntityRuntimeId);

#ifdef DEV_CONFIGURATION
        auto& vector = mMutableEntityIdToComponentViews[mutableEntityRuntimeId];
        for (auto it = vector.begin(); it != vector.end(); ++it)
        {
            if (it->GetComponentId().id == componentId.id) 
            {
                vector.erase(it);
                return;
            }
        }
#endif // DEV_CONFIGURATION.
    });
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
    mFunctionsToExecutePostUpdate.emplace_back([&, entityId]
    {
        SCARLENT_ASSERT(mEntityIdToArchetypeComponentArray.contains(entityId) && "Trying to remove an entity that has not been created.");

        Archetype* array = mEntityIdToArchetypeComponentArray[entityId];
        mEntityIdToArchetypeComponentArray.erase(entityId);

        array->RemoveEntity(entityId);

#ifdef DEV_CONFIGURATION
        mEntityIdToComponentViews.erase(entityId);
#endif // DEV_CONFIGURATION.
    });
}

template <typename...Components>
inline auto ComponentManager::GetComponents()
{
    std::vector<std::tuple<Components&...>> result;

    const uint64 componentsBitset = (Registry::Instance().GetOrRegisterComponentId<Components>().bitmask | ...);

    for (auto& [bitset, archetype] : mArchetypeComponents)
    {
        if ((bitset & componentsBitset) == componentsBitset)
        {
            auto archetypeArrays = archetype->GetComponentArrays<Components...>();

            for (size_t i = 0; i < std::get<0>(archetypeArrays)->componentArray.size(); ++i)
            {
                result.emplace_back(
                    std::apply([i](auto*... arrays) { return std::tie(arrays->componentArray[i]...); }, archetypeArrays)
                );
            }
        }
    }

    for (auto& [bitset, sparseSetEntities] : mComponentBitsetToMutableEntities)
    {
        if ((bitset & componentsBitset) == componentsBitset)
        {
            auto components = std::forward_as_tuple<SparseSet<Components, Registry::COMPONENT_BITSET_PAGE_SIZE>&...>(
                *static_cast<SparseSet<Components, Registry::COMPONENT_BITSET_PAGE_SIZE>*>(mComponentIdToSparseSetArray[Registry::Instance().GetOrRegisterComponentId<Components>().id])...);

            for (const uint32_t entityId : sparseSetEntities)
            {
                result.emplace_back(
                    std::apply([entityId](auto&... sets) { return std::tie(sets[entityId]...); }, components)
                );
            }
        }
    }

    return result;
}

template <typename Component, typename...ArchetypeComponents>
inline Component& ComponentManager::GetComponent(const Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToArchetypeComponentArray.contains(entityId) && "Trying to retrieve a component that has not been created.");

    Archetype* array = mEntityIdToArchetypeComponentArray[entityId];

    return array->GetComponent<Component>(entityId);
}

} // Namespace ScarlEnt.
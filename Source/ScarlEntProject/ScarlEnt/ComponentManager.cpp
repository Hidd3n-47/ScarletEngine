#include "ComponentManager.h"

#include <ranges>

namespace ScarlEnt
{

ComponentManager::~ComponentManager()
{
    // Archetypes.
    for (const auto& array : mArchetypeComponents | std::views::values)
    {
        delete array;
    }

    // Sparse-Sets.
    for (const auto& sparse : mSparseSetComponentArrays)
    {
        delete sparse;
    }

    // Systems.
    for (const ISystem* system : mSystems)
    {
        delete system;
    }
}

void ComponentManager::RemoveAllSystems()
{
    for (const ISystem* system : mSystems)
    {
        delete system;
    }

    for (const ISystem* system : mFixedSystems)
    {
        delete system;
    }

    mSystems.clear();
    mFixedSystems.clear();
}

void ComponentManager::Update()
{
    for (ISystem* system : mSystems)
    {
        for (auto& [bitset, archetype] : mArchetypeComponents)
        {
            if ((bitset & system->componentBitset) == system->componentBitset)
            {
                system->UpdateArchetype(archetype);
            }
        }

        for (auto& [bitset, sparseSetEntities] : mComponentBitsetToMutableEntities)
        {
            if ((bitset & system->componentBitset) == system->componentBitset)
            {
                system->UpdateSparseSet(sparseSetEntities, mComponentIdToSparseSetArray);
            }
        }

    }
}

void ComponentManager::PostUpdate()
{
    for (const std::function<void()>& func : mFunctionsToExecutePostUpdate)
    {
        func();
    }

    mFunctionsToExecutePostUpdate.clear();
}

void ComponentManager::FixedUpdate()
{
    for (ISystem* system : mFixedSystems)
    {
        for (auto& [bitset, archetype] : mArchetypeComponents)
        {
            if ((bitset & system->componentBitset) == system->componentBitset)
            {
                system->UpdateArchetype(archetype);
            }
        }

        for (auto& [bitset, sparseSetEntities] : mComponentBitsetToMutableEntities)
        {
            if ((bitset & system->componentBitset) == system->componentBitset)
            {
                system->UpdateSparseSet(sparseSetEntities, mComponentIdToSparseSetArray);
            }
        }

    }
}

EntityId ComponentManager::AddMutableEntity()
{
    constexpr uint64 noComponentBitset = 0;
    const uint64 runtimeId = mMutableEntitiesId++;

    mComponentBitsetToMutableEntities[noComponentBitset].emplace(runtimeId);
    mMutableEntityToComponentBitset.Add(runtimeId, noComponentBitset);

    return EntityId{ .runtimeId = runtimeId, .uniqueId = Scarlet::Ulid{} };
}

void ComponentManager::RemoveMutableEntity(const uint64 mutableEntityRuntimeId)
{
    mFunctionsToExecutePostUpdate.emplace_back([&, mutableEntityRuntimeId]
    {
        const uint64 bitset = mMutableEntityToComponentBitset[mutableEntityRuntimeId];

        for (uint32 i{ 0 }; i < sizeof(bitset); ++i)
        {
            if (const uint64 bitmask = (1ull << i) & bitset; bitmask)
            {
                const uint32 componentId = Registry::Instance().GetComponentIdFromBitmask(bitmask);

                mComponentIdToSparseSetArray[componentId]->Remove(mutableEntityRuntimeId);
            }
        }

        mComponentBitsetToMutableEntities[bitset].erase(mutableEntityRuntimeId);
        mMutableEntityToComponentBitset.Remove(mutableEntityRuntimeId);

#ifdef DEV_CONFIGURATION
        mMutableEntityIdToComponentViews.erase(mutableEntityRuntimeId);
#endif // DEV_CONFIGURATION.
    });
}

} // Namespace ScarlEnt.
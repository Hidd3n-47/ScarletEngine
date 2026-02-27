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

MutableEntityId ComponentManager::AddMutableEntity()
{
    constexpr uint64 noComponentBitset = 0;
    const uint32 runtimeId = mMutableEntitiesId++;

    mComponentBitsetToMutableEntities[noComponentBitset].emplace(runtimeId);
    mMutableEntityToComponentBitset.Add(runtimeId, noComponentBitset);

    return MutableEntityId{ .runtimeId = runtimeId, .uniqueId = Scarlet::Ulid{} };
}

void ComponentManager::RemoveMutableEntity(const uint32 mutableEntityRuntimeId)
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
}

} // Namespace ScarlEnt.
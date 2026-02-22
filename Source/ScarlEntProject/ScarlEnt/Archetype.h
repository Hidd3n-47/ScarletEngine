#pragma once

#include <tuple>

#include <ScarletCore/Ulid.h>
#include <ScarletCore/vector.h>
#include <ScarletCore/unordered_map.h>
#include <ScarletCore/PrimitiveTypes.h>

#include "Defines.h"
#include "IComponentArray.h"

namespace ScarlEnt
{
/**
 * @class Archetype: An archetype is defined to be a set of components that entities can contain. \nn
 * Archetypes are not runtime-mutable and hence once set for an entity, will remain fixed for the lifespan of the entity. \nn
 * Systems can query different permutations of components, and will iterate through all archetypes with the given permutation of components.
 * @tparam Components The components the archetype contain.
 */
template<typename...Components>
class Archetype : public IComponentArray
{
public:
    /**
     * @brief Get the component for a given entityId.
     * @tparam Component The component type.
     * @param entityId The ID of the entity whose component is being requested.
     * @return A reference to the component of the requested identifier.
     */
    template <typename Component>
    inline Component& GetComponent(const Scarlet::Ulid entityId)
    {
        SCARLENT_ASSERT(mEntityIdToIndex.contains(entityId) && "Trying to retrieve a component for an entity that is not added to this archetype.");

        const uint32 index = mEntityIdToIndex[entityId];

        return std::get<vector<Component>>(mComponents)[index];
    }

    /**
     * @brief Add an entity to the archetype.
     * @param entityId: The ID of the entity being added to the archetype.
     * @param componentValues Values that the components are initialised to.
     */
    inline void AddEntity(const Scarlet::Ulid entityId, Components&&... componentValues)
    {
        mEntityIds.emplace_back(entityId);
        mEntityIdToIndex[entityId] = mEntityIds.size() - 1;

        std::apply([&](auto&... vectors)
            {
                (vectors.emplace_back(componentValues), ...);
            }, mComponents);
    }

    /**
     * @brief Remove an entity from the archetype.
     * @param entityId The ID of the entity that is being removed from the archetype.
     */
    inline void RemoveEntity(const Scarlet::Ulid entityId)
    {
        SCARLENT_ASSERT(mEntityIdToIndex.contains(entityId) && "Trying to remove an entity that is not added to this archetype.");

        const uint32 index = mEntityIdToIndex[entityId];

        mEntityIdToIndex[mEntityIds.back()] = index;
        mEntityIds[index]  = mEntityIds.back();

        mEntityIds.pop_back();

        std::apply([&](auto&... vectors)
            {
                ((vectors[index] = std::move(vectors.back()), vectors.pop_back()), ...);
            }, mComponents);

        mEntityIdToIndex.erase(entityId);
    }

private:
    std::tuple<vector<Components>...> mComponents;

    vector<Scarlet::Ulid> mEntityIds;
    unordered_map<Scarlet::Ulid, uint32> mEntityIdToIndex;
};

} // Namespace ScarlEnt.

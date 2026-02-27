#pragma once

#include <functional>

#include <ScarletCore/PrimitiveTypes.h>

#include "Archetype.h"
#include "SparseSet.h"

namespace ScarlEnt
{

class Scene;
class Archetype;

/**
 * @struct ISystem: An interface for systems, this interface is used to strip the type information out of the system allowing for dynamic storage of systems.\\n
 * A system is something that will be used to manipulate component data in bulk.
 */
struct ISystem
{
    virtual ~ISystem() = default;

    /**
     * @brief Update the system by calling the systems update function on a specific archetype.
     * @param archetype The archetype of components that the systems update function will act on.
     */
    virtual void UpdateArchetype(Archetype* archetype) = 0;

    /**
     * @brief Update the system by calling the systems update function on specific subset of components in the \ref SparseSet.
     * @param entityIds The runtime entity identifiers of mutable entities with the subset of components.
     * @param componentSparseSet The \ref SparseSet to the component arrays.
     */
    virtual void UpdateSparseSet(const std::unordered_set<uint32>& entityIds, SparseSet<ISparseComponentArray*, Registry::COMPONENTS_PAGE_SIZE>& componentSparseSet) = 0;

    uint64 componentBitset = 0;
};

/**
 * @class System: A structure representing a system (\ref ISystem) with its type information. This type information can be used to get all the components of the type \\n
 * from the archetype and update.
 * @tparam Components The components that the system requires as a part of its update.
 */
template <typename...Components>
struct System : ISystem
{
    explicit System(const std::function<void(Components&...)>& updateFunction) noexcept
        : updateFunction(updateFunction) 
    {
        componentBitset = (Registry::Instance().GetOrRegisterComponentId<Components>().bitmask | ...);
    }

    /**
     * @copydoc ISystem::UpdateArchetype
     */
    void UpdateArchetype(Archetype* archetype) override
    {
        auto components = archetype->GetComponentArrays<Components...>();
        const size_t size = archetype->GetSize();

        std::apply(
            [&](auto&... arrays)
            {
                for (std::size_t i = 0; i < size; ++i)
                {
                    updateFunction(arrays->componentArray[i]...);
                }
            },
            components
        );
    }

    /**
     * @copydoc ISystem::UpdateSparseSet
     */
    void UpdateSparseSet(const std::unordered_set<uint32>& entityIds, SparseSet<ISparseComponentArray*, Registry::COMPONENTS_PAGE_SIZE>& componentSparseSet) override
    {
        auto components = std::forward_as_tuple<SparseSet<Components, Registry::COMPONENT_BITSET_PAGE_SIZE>&...>(
            *static_cast<SparseSet<Components, Registry::COMPONENT_BITSET_PAGE_SIZE>*>(componentSparseSet[Registry::Instance().GetOrRegisterComponentId<Components>().id])...);

        std::apply(
            [&](auto&... sparseSets)
            {
                for (const uint32 entityId : entityIds)
                {
                    updateFunction(sparseSets[entityId]...);
                }
            },
            components
        );
    }

    std::function<void(Components&...)> updateFunction;
};

} // Namespace ScarlEnt.
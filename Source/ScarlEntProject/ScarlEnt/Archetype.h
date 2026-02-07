#pragma once

#include <tuple>

#include <ScarletCore/vector.h>
#include <ScarletCore/PrimitiveTypes.h>

namespace ScarlEnt
{
/**
 * @class Archetype: An archetype is defined to be a set of components that entities can contain. \nn
 * Archetypes are not runtime-mutable and hence once set for an entity, will remain fixed for the lifespan of the entity. \nn
 * Systems can query different permutations of components, and will iterate through all archetypes with the given permutation of components.
 * @tparam Components The components the archetype contain.
 */
template<typename...Components>
struct Archetype
{
    /**
     * @brief Get the component for a given index.
     * @tparam Component The component type.
     * @param index The index of requested component.
     * @return A pointer to the component at the requested index.
     */
    template <typename Component>
    inline Component* GetComponent(const uint32 index)
    {
        return &std::get<vector<Component>>(components)[index];
    }

    /**
     * @brief Add an entity to the archetype.
     * @param componentValues Values that the components are initialised to.
     */
    inline void AddEntity(Components&&... componentValues)
    {
        std::apply([&](auto&... vecs)
            {
                (vecs.emplace_back(componentValues), ...);
            }, components);
    }

    /**
     * @brief Remove an entity from the archetype.
     * @param index The index of the entity that is being removed from the archetype.
     */
    inline void RemoveEntity(const uint32 index)
    {
        std::apply([&](auto&... vecs)
            {
                ((vecs[index] = std::move(vecs.back()), vecs.pop_back()), ...);
            }, components);
    }

    std::tuple<vector<Components>...> components;
};

} // Namespace ScarlEnt.

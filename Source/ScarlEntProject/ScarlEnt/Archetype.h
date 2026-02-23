#pragma once

#include <tuple>

#include <ScarletCore/Ulid.h>
#include <ScarletCore/vector.h>
#include <ScarletCore/unordered_map.h>

#include "Defines.h"
#include "Registry.h"
#include "IComponentArray.h"

namespace ScarlEnt
{
/**
 * @class Archetype: An archetype is defined to be a set of components that entities can contain. \nn
 * Archetypes are not runtime-mutable and hence once set for an entity, will remain fixed for the lifespan of the entity. \nn
 * Systems can query different permutations of components, and will iterate through all archetypes with the given permutation of components.
 */
class Archetype
{
public:
    /**
     * @brief Construct an archetype and add the first entity into the archetype.
     * @tparam Components The component types the archetype contains.
     * @param entityId The entity identifier that is being added to the archetype.
     * @param values The values being forwarded to the constructors of the components.
     */
    template<typename...Components>
    Archetype(const Scarlet::Ulid entityId, Components&&...values);

    /**
     * @brief Get the component for a given entityId.
     * @tparam Component The component type.
     * @param entityId The ID of the entity whose component is being requested.
     * @return A reference to the component of the requested identifier.
     */
    template <typename Component>
    [[nodiscard]] Component& GetComponent(const Scarlet::Ulid entityId);

    /**
     * @brief Add an entity to the archetype.
     * @param entityId: The ID of the entity being added to the archetype.
     * @param componentValues Values that the components are initialised to.
     */
    template<typename...Components>
    void AddEntity(const Scarlet::Ulid entityId, Components&&... componentValues);

    /**
     * @brief Remove an entity from the archetype.
     * @param entityId The ID of the entity that is being removed from the archetype.
     */
    void RemoveEntity(const Scarlet::Ulid entityId);

    /**
     * @brief Get component arrays of the requested types on the archetype.
     * @tparam Components The component types being requested from the archetype.
     * @return A tuple of the \ref ComponentArray 's (with types).
     */
    template <typename...Components>
    [[nodiscard]] auto GetComponentArrays();

    /**
     * @brief Get a component array of the requested type on the archetype.
     * @tparam Component The component type being requested from the archetype.
     * @return A \ref ComponentArray of the requested component type.
     */
    template <typename Component>
    [[nodiscard]] ComponentArray<Component>* GetComponentArray();

private:
    vector<IComponentArray*> mComponents;

    vector<Scarlet::Ulid> mEntityIds;
    unordered_map<Scarlet::Ulid, size_t> mEntityIdToIndex;

    unordered_map<const char*, size_t> mComponentIdToArrayIndex;

#ifdef DEV_CONFIGURATION
    uint64 mArchetypeComponentBitmask = 0;
#endif // DEV_CONFIGURATION.
};

/* ============================================================================================================================== */

template<typename...Components>
inline Archetype::Archetype(const Scarlet::Ulid entityId, Components&&...values)
{
#ifdef DEV_CONFIGURATION
    mArchetypeComponentBitmask = (Registry::Instance().GetOrRegisterComponentBit<Components>() | ...);
#endif // DEV_CONFIGURATION.

    size_t index = 0;
    ((mComponentIdToArrayIndex[Registry::GetComponentTypeId<Components>()] = index++, mComponents.emplace_back(new ComponentArray<Components>())), ...);

    AddEntity(entityId, std::forward<Components>(values)...);
}

template <typename Component>
inline Component& Archetype::GetComponent(const Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToIndex.contains(entityId) && "Trying to retrieve a component for an entity that is not added to this archetype.");

    const size_t index = mEntityIdToIndex[entityId];

    return static_cast<ComponentArray<Component>*>(mComponents[mComponentIdToArrayIndex[Registry::GetComponentTypeId<Component>()]])->componentArray[index];
}

template<typename...Components>
inline void Archetype::AddEntity(const Scarlet::Ulid entityId, Components&&... componentValues)
{
    SCARLENT_ASSERT((Registry::Instance().GetOrRegisterComponentBit<Components>() | ...) == mArchetypeComponentBitmask && "Adding an entity with a different archetype component definition.");

    mEntityIds.emplace_back(entityId);
    mEntityIdToIndex[entityId] = mEntityIds.size() - 1;

    ((static_cast<ComponentArray<Components>*>(mComponents[mComponentIdToArrayIndex[Registry::GetComponentTypeId<Components>()]])
        ->AddComponent(std::forward<Components>(componentValues))), ...);
}

inline void Archetype::RemoveEntity(const Scarlet::Ulid entityId)
{
    SCARLENT_ASSERT(mEntityIdToIndex.contains(entityId) && "Trying to remove an entity that is not added to this archetype.");

    const size_t index = mEntityIdToIndex[entityId];

    mEntityIdToIndex[mEntityIds.back()] = index;
    mEntityIds[index] = mEntityIds.back();

    mEntityIds.pop_back();

    for (IComponentArray* array : mComponents)
    {
        array->RemoveComponent(index);
    }

    mEntityIdToIndex.erase(entityId);
}

template <typename...Components>
inline auto Archetype::GetComponentArrays()
{
    return std::make_tuple(GetComponentArray<Components>()...);
}

template <typename Component>
inline ComponentArray<Component>* Archetype::GetComponentArray()
{
    const char* const componentTypeId = Registry::GetComponentTypeId<Component>();
    SCARLENT_ASSERT(mComponentIdToArrayIndex.contains(componentTypeId) && "Archetype does not contain component type that the array is being requested for.");

    return static_cast<ComponentArray<Component>*>(mComponents[mComponentIdToArrayIndex[componentTypeId]]);
}

} // Namespace ScarlEnt.

#pragma once

#include <ScarletCore/Ulid.h>
#include <ScarletCore/WeakHandle.h>

#include "IEntityHandle.h"
#include "ComponentManager.h"

namespace ScarlEnt
{

/**
 * @class EntityHandle: A handle to deal with entities. This is a wrapper class used to assist with interfacing with entities. \\n
 * Entities are just an identifier and so it is impossible to say \code entity.GetComponent\endcode . A wrapper allows the user to \\n
 * interface with the "entity" object. This allows for easier interfacing due to not needing to pass in the archetype component types for all functions.
 * @tparam ArchetypeComponents The types of the components in the archetype.
 * @remark Since the entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
 * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
 * of the scene.
 */
template <typename...ArchetypeComponents>
class EntityHandle : public IEntityHandle
{
    friend class Scene;
public:
    EntityHandle() = default;

    /**
     * @brief Create an instance of an \ref EntityHandle.
     * @tparam Args The types of the components in the archetype.
     * @param componentManagerRef A weak reference to the component manager responsible for the creation of the entity components.
     * @remark Since the entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
     * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
     * of the scene.
     * @param args The arguments forwarded to the constructors of the components.
     */
    template <typename...Args>
    EntityHandle(const Scarlet::WeakHandle<ComponentManager> componentManagerRef, Args&&...args)
        : mComponentManagerRef(componentManagerRef)
        , mIsValid(true)
    {
        mEntityId = mComponentManagerRef->AddEntity<ArchetypeComponents...>(std::forward<Args>(args)...);
    }

    /**
     * @brief Create an instance of an \ref EntityHandle.
     * @param componentManagerRef A weak reference to the component manager responsible for the creation of the entity components.
     * @remark Since the entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
     * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
     * of the scene.
     */
    EntityHandle(const Scarlet::WeakHandle<ComponentManager> componentManagerRef)
        : mComponentManagerRef(componentManagerRef)
        , mIsValid(true)
    {
        mEntityId = mComponentManagerRef->AddEntity<ArchetypeComponents...>((ArchetypeComponents{}, ...));
    }

    /**
     * @brief Get a component attached to an entity.
     * @tparam Component The type of the component being requested.
     * @return A reference to the component.
     */
    template <typename Component>
    Component& GetComponent()
    {
        return mComponentManagerRef->GetComponent<Component, ArchetypeComponents...>(mEntityId.uniqueId);
    }

    /**
     * @brief Remove the immutable entity from the scene.
     */
    inline void DestroyEntity()
    {
        mIsValid = false;
        mComponentManagerRef->RemoveEntity<ArchetypeComponents...>(mEntityId.uniqueId);
    }

    [[nodiscard]] inline EntityId GetEntityId() const final { return mEntityId; }
    [[nodiscard]] inline bool     IsValid()     const       { return mIsValid;  }

    DEBUG([[nodiscard]] inline const vector<ComponentView>& GetComponentViews() override { return mComponentManagerRef->GetEntityComponentView(mEntityId.uniqueId); })
private:
    EntityId mEntityId;
    Scarlet::WeakHandle<ComponentManager> mComponentManagerRef;

    bool mIsValid = false;
};

} // Namespace ScarlEnt.
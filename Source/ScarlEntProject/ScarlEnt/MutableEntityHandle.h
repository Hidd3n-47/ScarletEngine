#pragma once

#include <ScarletCore/WeakHandle.h>

#include "Defines.h"
#include "IEntityhandle.h"
#include "ComponentManager.h"

namespace ScarlEnt
{

class Scene;

/**
 * @class MutableEntityHandle: A handle to interact with mutable entities. This is a wrapper class used to assist with interfacing with entities. \\n
 * Entities are just an identifier and so it is impossible to say \code entity.GetComponent\endcode . A wrapper allows the user to \\n
 * interface with the "mutable entity" object.
 * @remark Since the mutable entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
 * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
 * of the scene.
 */
class SCARLENT_API MutableEntityHandle : public IEntityHandle
{
    friend class Scene;
public:
    MutableEntityHandle() = default;

#ifdef DEV_CONFIGURATION
    /**
     * @brief Create an instance of an \ref MutableEntityHandle.
     * @param parent The scene the entity is attached to.
     * @param componentManagerRef A weak reference to the component manager responsible for the creation of the entity components.
     * @remark Since the mutable entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
     * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
     * of the scene.
     */
    inline MutableEntityHandle(const Scarlet::WeakHandle<Scene> parent, const Scarlet::WeakHandle<ComponentManager> componentManagerRef)
        : mParent(parent)
        , mComponentManagerRef(componentManagerRef)
        , mIsValid(true)
    {
        mEntityId = mComponentManagerRef->AddMutableEntity();
    }
#endif // DEV_CONFIGURATION.
    /**
     * @brief Create an instance of an \ref MutableEntityHandle.
     * @param componentManagerRef A weak reference to the component manager responsible for the creation of the entity components.
     * @remark Since the mutable entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
     * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
     * of the scene.
     */
    inline MutableEntityHandle(const Scarlet::WeakHandle<ComponentManager> componentManagerRef)
        : mComponentManagerRef(componentManagerRef)
        , mIsValid(true)
    {
        mEntityId = mComponentManagerRef->AddMutableEntity();
    }

    /**
     * @brief Add a component to the mutable entity.
     * @tparam T The component being added to the entity.
     * @tparam Args The argument types for construction of the component.
     * @param args The arguments forwarded to the constructor of the component.
     */
    template <typename T, typename...Args>
    inline T& AddComponent(Args&&...args)
    {
        SCARLENT_ASSERT(mIsValid && "Trying to add component on mutable entity that has been marked as invalid.");

        T& component = mComponentManagerRef->AddComponent<T>(mEntityId.runtimeId, std::forward<Args>(args)...);
        component.SetEntityUniqueId(mEntityId.uniqueId);

        return component;
    }

    /**
     * @brief Remove a component from the mutable entity.
     * @tparam T The component being removed from the entity.
     */
    template <typename T>
    inline void RemoveComponent()
    {
        SCARLENT_ASSERT(mIsValid && "Trying to remove component on mutable entity that has been marked as invalid.");

        mComponentManagerRef->RemoveComponent<T>(mEntityId.runtimeId);
    }

    /**
     * @brief Destroy and remove the entity from the scene.
     */
    void DestroyEntity();

    /**
     * @brief Get a component attached to an entity.
     * @tparam Component The type of the component being requested.
     * @return A reference to the component.
     */
    template <typename Component>
    [[nodiscard]] inline Component& GetComponent()
    {
        SCARLENT_ASSERT(mIsValid && "Trying to get component on mutable entity that has been marked as invalid.");
        return mComponentManagerRef->GetMutableEntityComponent<Component>(mEntityId.runtimeId);
    }

    /**
     * @brief Get a constance reference to a component attached to an entity.
     * @tparam Component The type of the component being requested.
     * @return A constant reference to the component.
     */
    template <typename Component>
    [[nodiscard]] inline const Component& GetComponent() const
    {
        DEBUG(SCARLENT_ASSERT(mIsValid && "Trying to get component on mutable entity that has been marked as invalid."));
        return GetComponent<Component>();
    }

    /**
     * @brief Get if the mutable entity has a specific component.
     * @tparam Component The component being requested if the entity contains it.
     * @return \c true if the entity has this component attached, \c false otherwise.
     */
    template <typename Component>
    [[nodiscard]] inline bool HasComponent()
    {
        const uint64 bitmask = Registry::Instance().GetOrRegisterComponentId<Component>().bitmask;

        return bitmask & mComponentManagerRef->GetMutableEntityComponentBitset(mEntityId.runtimeId);
    }

#ifdef SCARLENT_TEST
    [[nodiscard]] inline Scarlet::WeakHandle<ComponentManager> GetComponentManagerRef() const { return mComponentManagerRef; }
#endif // SCARLENT_TEST.

    [[nodiscard]] inline uint64   GetComponentBitset()      { return mComponentManagerRef->GetMutableEntityComponentBitset(mEntityId.runtimeId); }
    [[nodiscard]] inline bool     IsValid()     const       { return mIsValid; }
    [[nodiscard]] inline bool     IsMutable()   const final { return true; }
    [[nodiscard]] inline EntityId GetEntityId() const final { return mEntityId; }

#ifdef DEV_CONFIGURATION
    [[nodiscard]] inline const vector<ComponentView>& GetComponentViews() final { return mComponentManagerRef->GetMutableEntityComponentView(mEntityId.runtimeId); }
#endif // DEV_CONFIGURATION.
private:
    EntityId mEntityId;
    DEBUG(Scarlet::WeakHandle<Scene> mParent);
    Scarlet::WeakHandle<ComponentManager> mComponentManagerRef;

    bool mIsValid = false;
};

} // Namespace ScarlEnt.
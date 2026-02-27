#pragma once

#include <ScarletCore/WeakHandle.h>

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
class SCARLENT_API MutableEntityHandle
{
    friend class Scene;
public:
    /**
     * @brief Create an instance of an \ref MutableEntityHandle.
     * @param componentManagerRef A weak reference to the component manager responsible for the creation of the entity components.
     * @remark Since the mutable entity handle holds a reference to the component manager, it is only valid if the component manager is valid. \\n
     * The component manager is valid for the life-time of the scene. This coupling of lifetime is okay as entities are only valid for the life-time \\n
     * of the scene.
     */
    MutableEntityHandle(const Scarlet::WeakHandle<ComponentManager> componentManagerRef)
        : mComponentManagerRef(componentManagerRef)
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
    void AddComponent(Args&&...args)
    {
        DEBUG(SCARLENT_ASSERT(mIsValid && "Trying to add component on mutable entity that has been marked as invalid."));

        mComponentManagerRef->AddComponent<T>(mEntityId.runtimeId, std::forward<Args>(args)...);
    }

    /**
     * @brief Remove a component from the mutable entity.
     * @tparam T The component being removed from the entity.
     */
    template <typename T>
    void RemoveComponent()
    {
        DEBUG(SCARLENT_ASSERT(mIsValid && "Trying to remove component on mutable entity that has been marked as invalid."));

        mComponentManagerRef->RemoveComponent<T>(mEntityId.runtimeId);
    }

    /**
     * @brief Destroy and remove the entity from the scene.
     */
    void DestroyEntity()
    {
        DEBUG(SCARLENT_ASSERT(mIsValid && "Trying to remove entity on mutable entity that has been marked as invalid."));
        DEBUG(mIsValid = false);

        mComponentManagerRef->RemoveMutableEntity(mEntityId.runtimeId);
    }

    /**
     * @brief Get a component attached to an entity.
     * @tparam Component The type of the component being requested.
     * @return A reference to the component.
     */
    template <typename Component>
    [[nodiscard]] Component& GetComponent()
    {
        DEBUG(SCARLENT_ASSERT(mIsValid && "Trying to get component on mutable entity that has been marked as invalid."));
        return mComponentManagerRef->GetMutableEntityComponent<Component>(mEntityId.runtimeId);
    }

    /**
     * @brief Get a constance reference to a component attached to an entity.
     * @tparam Component The type of the component being requested.
     * @return A constant reference to the component.
     */
    template <typename Component>
    [[nodiscard]] const Component& GetComponent() const
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

        return bitmask & mComponentManagerRef->GetMutableEntityComponentBitset(mEntityId.runtimeId);;
    }

#ifdef SCARLENT_TEST
    [[nodiscard]] inline MutableEntityId GetId() const { return mEntityId; }
    [[nodiscard]] inline uint64 GetComponentBitset() { return mComponentManagerRef->GetMutableEntityComponentBitset(mEntityId.runtimeId); }
    [[nodiscard]] inline Scarlet::WeakHandle<ComponentManager> GetComponentManagerRef() const { return mComponentManagerRef; }
#endif // SCARLENT_TEST.
private:
    MutableEntityId mEntityId;
    Scarlet::WeakHandle<ComponentManager> mComponentManagerRef;

    DEBUG(bool mIsValid = true);
};

} // Namespace ScarlEnt.
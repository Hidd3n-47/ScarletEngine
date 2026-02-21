#pragma once

#include <ScarletCore/Ulid.h>

#include "ComponentManager.h"

namespace ScarlEnt
{
/**
 * @class Scene: A class to hold a collection of entities, components and handles system updates.
 */
class SCARLENT_API Scene
{
    friend class Registry;
public:
    template <typename...ArchetypeComponents, typename...Args>
    [[nodiscard]] Scarlet::Ulid AddEntity(Args&& ...args);

    template <typename...ArchetypeComponents>
    void RemoveEntity(const Scarlet::Ulid entityId);

    template <typename Component, typename...ArchetypeComponents>
    [[nodiscard]] Scarlet::WeakHandle<Component> GetComponent(const Scarlet::Ulid entityId);

    /** @brief Get the friendly name of the scene */
    [[nodiscard]] inline std::string_view GetFriendlyName() const { return mFriendlyName; }

    /** @brief Get the index of the scene in the registry. */
    [[nodiscard]] inline size_t GetRegistryIndex() const { return mRegistryIndex; }
private:
    Scene(const std::string_view friendlyName, const size_t index);

    std::string mFriendlyName;
    size_t      mRegistryIndex;

    ComponentManager mComponentManager;
};

/* ============================================================================================================================== */

template <typename... ArchetypeComponents, typename... Args>
inline Scarlet::Ulid Scene::AddEntity(Args&&... args)
{
    return mComponentManager.AddEntity<ArchetypeComponents...>(std::forward<Args>(args)...);
}

template <typename...ArchetypeComponents>
void Scene::RemoveEntity(const Scarlet::Ulid entityId)
{
    mComponentManager.RemoveEntity<ArchetypeComponents...>(entityId);
}

template <typename Component, typename...ArchetypeComponents>
inline Scarlet::WeakHandle<Component> Scene::GetComponent(const Scarlet::Ulid entityId)
{
    return mComponentManager.GetComponent<Component, ArchetypeComponents...>(entityId);
}

} // Namespace ScarlEnt.
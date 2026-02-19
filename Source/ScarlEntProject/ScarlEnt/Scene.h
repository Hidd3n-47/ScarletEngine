#pragma once

#include <string>

#include <ScarletCore/Ulid.h>

namespace ScarlEnt
{
/**
 * @class Scene: A class to hold a collection of entities, components and handles system updates.
 */
class Scene
{
    friend class Registry;
public:
    Scarlet::Ulid AddEntity() { return {}; }

    /** @brief Get the friendly name of the scene */
    [[nodiscard]] inline std::string_view GetFriendlyName() const { return mFriendlyName; }

    /** @brief Get the index of the scene in the registry. */
    [[nodiscard]] inline size_t GetRegistryIndex() const { return mRegistryIndex; }
private:
    Scene(const std::string_view friendlyName, const size_t index);

    std::string mFriendlyName;
    size_t      mRegistryIndex;
};

} // Namespace ScarlEnt.
#include "Scene.h"

namespace ScarlEnt
{

Scene::Scene(const std::string_view friendlyName, const size_t index)
    : mFriendlyName(friendlyName)
    , mRegistryIndex(index)
{
    // Empty.
}

} // Namespace ScarlEnt.
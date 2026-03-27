#pragma once

#ifdef DEV_CONFIGURATION

#include <string>
#include <functional>

#include <ScarletCore/unordered_map.h>

#include "IEntityhandle.h"
#include "RTTI/Property.h"

namespace ScarlEnt
{
using PropertyMap = unordered_map<std::string, Property>;

struct TypelessComponentFunctions
{
    std::function<PropertyMap* (IEntityHandle*)> addComponent;
    std::function<bool         (IEntityHandle*)> hasComponent;
    std::function<void         (IEntityHandle*)> removeComponent;
};

} // Namespace ScarlEnt.

#endif // DEV_CONFIGURATION.

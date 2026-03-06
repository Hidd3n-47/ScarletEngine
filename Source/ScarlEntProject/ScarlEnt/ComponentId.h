#pragma once

#include <string_view>

#include <ScarletCore/PrimitiveTypes.h>

namespace ScarlEnt
{

struct ComponentId
{
    uint32 id;
    uint64 bitmask;

#ifdef DEV_CONFIGURATION
    std::string_view name;
#endif // DEV_CONFIGURATION.
};

} // Namespace ScarlEnt.
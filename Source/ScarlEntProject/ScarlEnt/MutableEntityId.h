#pragma once

#include <ScarletCore/Ulid.h>
#include <ScarletCore/PrimitiveTypes.h>

namespace ScarlEnt
{

struct MutableEntityId
{
    uint32        runtimeId{};
    Scarlet::Ulid uniqueId{};
};

} // Namespace ScarlEnt.
#pragma once

#include <ScarletCore/Ulid.h>
#include <ScarletCore/PrimitiveTypes.h>

namespace ScarlEnt
{

struct EntityId
{
    uint64        runtimeId{};
    Scarlet::Ulid uniqueId {};
};

} // Namespace ScarlEnt.
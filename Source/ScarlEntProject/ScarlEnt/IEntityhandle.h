#pragma once

#include "EntityId.h"
#include "ScarlEnt/Defines.h"

#ifdef DEV_CONFIGURATION
#include <ScarletCore/vector.h>
#include <ScarlEnt/RTTI/ComponentView.h>
#endif // DEV_CONFIGURATION.

namespace ScarlEnt
{

class SCARLENT_API IEntityHandle
{
public:
    virtual ~IEntityHandle() = default;

    [[nodiscard]] inline virtual bool   IsMutable()     const { return false; }
    [[nodiscard]] inline virtual EntityId GetEntityId() const = 0;

#ifdef DEV_CONFIGURATION
    [[nodiscard]] inline virtual const vector<ComponentView>& GetComponentViews() = 0;
#endif // DEV_CONFIGURATION.
};

} // Namespace ScarlEnt.


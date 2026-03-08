#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarletCore/vector.h>
#include <ScarlEnt/RTTI/ComponentView.h>

namespace ScarlEnt
{


class IEntityHandle
{
public:
    virtual ~IEntityHandle() = default;

    [[nodiscard]] inline virtual bool   IsMutable() const { return false; }
    [[nodiscard]] inline virtual uint64 GetRuntimeId() const = 0;

    [[nodiscard]] inline virtual const vector<ComponentView>& GetComponentViews() = 0;
};


} // Namespace ScarlEnt.

#endif // DEV_CONFIGURATION.
#pragma once

namespace ScarlEnt
{

#ifdef DEV_CONFIGURATION

class IEntityHandle
{
public:
    virtual ~IEntityHandle() = default;

    [[nodiscard]] inline virtual bool IsMutable() const { return false; }
};

#endif // DEV_CONFIGURATION.

} // Namespace ScarlEnt.
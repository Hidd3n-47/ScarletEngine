#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarlEnt/IEntityhandle.h>

namespace Scarlet::Editor
{

/**
 * @class SelectionManager: A class used to manage what instances have been selected.
 */
class SelectionManager
{
public:

    [[nodiscard]] inline ScarlEnt::IEntityHandle* GetSelectedEntity() const { return mSelectedEntity; }
    inline void SetSelectedEntity(ScarlEnt::IEntityHandle* entity) { mSelectedEntity = entity; }
private:
    ScarlEnt::IEntityHandle* mSelectedEntity = nullptr;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
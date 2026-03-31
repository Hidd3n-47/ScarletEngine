#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarlEnt/IEntityhandle.h>

#include "ManipulatorScene.h"

namespace Scarlet::Editor
{

/**
 * @class SelectionManager: A class used to manage what instances have been selected.
 */
class SelectionManager
{
public:
    /**
     * @brief Manipulate the selected instances based off the \ref ManipulatorScene state.
     * @return \c true if the manipulation is in progress, \c false otherwise.
     */
    [[nodiscard]] inline bool ManipulatingSelected() const { return mManipulatorScene.Manipulate(mSelectedEntity); }

    [[nodiscard]] inline ManipulatorScene& GetManipulatorScene() { return mManipulatorScene; }
    [[nodiscard]] inline ScarlEnt::IEntityHandle* GetSelectedEntity() const { return mSelectedEntity; }
    inline void SetSelectedEntity(ScarlEnt::IEntityHandle* entity) { mSelectedEntity = entity; }
private:
    ScarlEnt::IEntityHandle* mSelectedEntity = nullptr;

    ManipulatorScene mManipulatorScene;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
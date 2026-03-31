#pragma once

namespace ScarlEnt
{
    class IEntityHandle;
} // Namespace ScarlEnt.

namespace Scarlet::Editor
{

/**
 * @enum ManManipulatorOperation: The type of operation that can manipulate instances: Translate, Rotate, Scale.
 */
enum class ManipulatorOperation : uint8
{
    TRANSLATE,
    ROTATE,
    SCALE,
};

/**
 * @class ManipulatorScene: An object that is responsible for tracking the current \ref ManipulatorOperation and manipulating the selection based off that operation.
 */
class ManipulatorScene
{
public:
    /**
     * @brief Handle and manipulate the selection based off the currently set \ref ManipulatorOperation.
     * @param selectedEntity The selection the manipulation will manipulate.
     * @return \c true if the manipulation is in progress, \c false otherwise.
     */
    bool Manipulate(ScarlEnt::IEntityHandle* selectedEntity) const;

    inline ManipulatorOperation GetManipulatorOperation() const { return mOperation; }
    inline void SetManipulatorOperation(const ManipulatorOperation operation) { mOperation = operation; }
private:
    ManipulatorOperation mOperation{ ManipulatorOperation::TRANSLATE };
};

} // Namespace Scarlet::Editor.
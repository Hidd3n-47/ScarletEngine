#include "ScarletEditorPch.h"
#include "ManipulatorScene.h"

#include <ImGuizmo/ImGuizmo.h>

#include <ScarletMath/Math.h>
#include <ScarletMath/Trig.h>

#ifdef DEV_CONFIGURATION

#include <ScarlEnt/MutableEntityHandle.h>

#include <ScarletCoreEcs/Components/Camera.h>
#include <ScarletCoreEcs/Components/Transform.h>

#include "EditorManager.h"

namespace Scarlet::Editor
{

bool ManipulatorScene::Manipulate(ScarlEnt::IEntityHandle* selectedEntity) const
{
    if (!selectedEntity)
    {
        return false;
    }

    Component::Camera& camera       = EditorManager::Instance().GetViewportCamera();
    Component::Transform& transform = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(selectedEntity)->GetComponent<Component::Transform>();

    Math::Mat4 matrix = Math::TransformAsMatrix(transform.translation,
            Math::Trig::RotationMatrix(transform.rotation.z, transform.rotation.x, transform.rotation.y), transform.scale);

    ImGuizmo::OPERATION operation;
    switch (mOperation)
    {
    case ManipulatorOperation::TRANSLATE:
        operation = ImGuizmo::OPERATION::TRANSLATE;
        break;
    case ManipulatorOperation::ROTATE:
        operation = ImGuizmo::OPERATION::ROTATE;
        break;
    case ManipulatorOperation::SCALE:
        operation = ImGuizmo::OPERATION::SCALE;
        break;
    default:
        operation = ImGuizmo::OPERATION::TRANSLATE;
        break;
    }

    const Math::Vec3 snappingAmount{ mIsSnapping ? mSnappingAmount : 0.0f };
    if (ImGuizmo::Manipulate(&camera.viewMatrix[0][0], &camera.projectionMatrix[0][0], operation, ImGuizmo::WORLD, &matrix[0][0], nullptr, &snappingAmount.x))
    {
        Math::Trig::DecomposeTransformMatrixDegrees(matrix, transform.translation, transform.rotation, transform.scale);
    }

    if (!ImGuizmo::IsOver())
    {
        return false;
    }

    return true;
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
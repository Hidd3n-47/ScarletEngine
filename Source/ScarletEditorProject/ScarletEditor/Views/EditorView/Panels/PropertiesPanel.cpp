#include "PropertiesPanel.h"

#ifdef DEV_CONFIGURATION

#include <string>

#include <imgui/imgui.h>

#include <ScarletEngine/RTTI/ReflectType.h>

#include "Views/EditorView/View/EditorView.h"

#include "UI/UiControl.h"

namespace Scarlet::Editor
{

void PropertiesPanel::Render()
{
    EditorView* editorView             = dynamic_cast<EditorView*>(mView);
    SelectionManager& selectionManager = editorView->GetSelectionManager();

    ScarlEnt::IEntityHandle* selected = selectionManager.GetSelectedEntity();

    if (!selected)
    {
        return;
    }

    for (auto& componentView : selected->GetComponentViews())
    {
        const std::string componentName = std::string{ componentView.GetComponentId().name };
        if (ImGui::CollapsingHeader(componentName.c_str(), ImGuiTreeNodeFlags_DefaultOpen))
        {
            for (auto& [propertyName, property] : *componentView.GetProperties())
            {
                const std::string entityName = "Entity";

                switch (property.GetType())
                {
                case ScarlEnt::PropertyType::VEC3:
                    UiControl::RenderVec3PropertyControl(property, { .propertyName = propertyName, .propertyId = entityName });
                    break;
                case ScarlEnt::PropertyType::VEC4:
                    UiControl::RenderVec4PropertyControl(property, { .propertyName = propertyName, .propertyId = entityName });
                    break;
                case ScarlEnt::PropertyType::QUAT:
                {
                    // Todo Christian Change this to render as a vec 3 using yaw, pitch and roll.
                    UiControl::RenderAngle(property, { .propertyName = propertyName, .propertyId = entityName });
                    break;
                }
                default:
                    SCARLET_ERROR("Failed to create a UI Control for the reflected type: {} on component {}", property.GetTypeAsString(), componentName);
                    break;
                }
            }
        }
    }

}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

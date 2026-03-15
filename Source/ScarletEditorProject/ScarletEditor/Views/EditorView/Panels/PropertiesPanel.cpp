#include "ScarletEditorPch.h"
#include "PropertiesPanel.h"

#ifdef DEV_CONFIGURATION
#include <ScarletLogger/Log.h>

#include <ScarlEnt/Registry.h>

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
                case ScarlEnt::PropertyType::FLOAT:
                    UiControl::RenderFloatPropertyControl(property, { .propertyName = propertyName, .propertyId = entityName });
                    break;
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
                case ScarlEnt::PropertyType::ASSET:
                    UiControl::RenderAssetPropertyControl(property, { .propertyName = propertyName, .propertyId = entityName });
                    break;
                default:
                    SCARLET_ERROR("Failed to create a UI Control for the reflected type: {} on component {}", property.GetTypeAsString(), componentName);
                    break;
                }
            }
        }
    }

    ImGui::NewLine();

    const float windowWidth     = ImGui::GetWindowSize().x;
    constexpr float buttonWidth = 150.0f;

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (ImGui::Button("Add Component", { buttonWidth, 25.0f }))
    {
        ImGui::OpenPopup("AddComponent");

        constexpr float yPadding = 5.0f;
        const ImVec2 buttonPos   = ImGui::GetItemRectMin();
        const ImVec2 buttonSize  = ImGui::GetItemRectSize();
        ImGui::SetNextWindowPos({ buttonPos.x + buttonSize.x * 0.5f, buttonPos.y + buttonSize.y + yPadding }, ImGuiCond_Always, { 0.5f, 0.0f });
    }

    if (ImGui::BeginPopup("AddComponent", ImGuiWindowFlags_NoMove))
    {
        constexpr float popupMinWidth  = 200.0f;
        constexpr float popupMaxHeight = 200.0f;

        ImGui::BeginChild("PopupItems", { popupMinWidth, popupMaxHeight }, true, ImGuiWindowFlags_HorizontalScrollbar);

        for (const std::string_view componentName : ScarlEnt::Registry::Instance().GetComponentToAddComponentMap() | std::views::keys)
        {
            const std::string compName = std::string{ componentName };

            if (ImGui::MenuItem(compName.c_str()))
            {
                ScarlEnt::Registry::Instance().AddComponentToHandle(compName.c_str(), selected);
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndChild();
        ImGui::EndPopup();
    }
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

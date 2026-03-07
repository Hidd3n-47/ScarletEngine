#include "ScarletEditorPch.h"
#include "UiControl.h"

#ifdef DEV_CONFIGURATION

#include <ScarletMath/Quat.h>

#include <ScarlEnt/RTTI/Property.h>

#include <ScarletEngine/RTTI/ReflectType.h>

#include "UiControlProperties.h"
#include "UiControlButtonProperties.h"

namespace Scarlet::Editor
{

void UiControl::RenderAngle(const ScarlEnt::Property& property, const UiControlProperties& controlProperties)
{
    Math::Quat value;
    ReflectType::SetValueFromString(value, property.GetPropertyValue());

    double yaw, pitch, roll;
    value.GetYawPitchRoll(yaw, pitch, roll);

    Math::Vec3 angle{ Math::Degrees(roll), Math::Degrees(pitch), Math::Degrees(yaw) };

    const std::string propertyId = controlProperties.propertyName + controlProperties.propertyId;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, controlProperties.propertyName.c_str());

    // Todo: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight - ImGui::CalcTextSize(controlProperties.propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", controlProperties.propertyName.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 4.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    UiControlButtonProperties buttonProperties{ .buttonSize = {lineHeight + 3.0f, lineHeight} };

    buttonProperties.buttonColor        = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f };
    RenderFloatPropertyControl("X", propertyId, angle.x, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f };
    RenderFloatPropertyControl("Y", propertyId, angle.y, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f };
    RenderFloatPropertyControl("Z", propertyId, angle.z, controlProperties, buttonProperties);

    ImGui::PopStyleVar(2);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

}

void UiControl::RenderVec3PropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties /* = {} */)
{
    Math::Vec3 value;
    ReflectType::SetValueFromString(value, property.GetPropertyValue());

    std::string propertyName = controlProperties.propertyName;
    if (controlProperties.firstLetterUppercase)
    {
        propertyName[0] = static_cast<char>(std::toupper(propertyName[0]));
    }

    const std::string propertyId = controlProperties.propertyName + controlProperties.propertyId;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, propertyName.c_str());

    // Todo Christian: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight - ImGui::CalcTextSize(propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", propertyName.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 4.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    UiControlButtonProperties buttonProperties{ .buttonSize = {lineHeight + 3.0f, lineHeight} };

    buttonProperties.buttonColor        = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f };
    RenderFloatPropertyControl("X", propertyId, value.x, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f };
    RenderFloatPropertyControl("Y", propertyId, value.y, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f };
    RenderFloatPropertyControl("Z", propertyId, value.z, controlProperties, buttonProperties);

    ImGui::PopStyleVar(2);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

    property.SetPropertyValue(ReflectType::GetStringFromValue(value));
}

// todo, this is a copy-paste of above, would be good to have this being generic with maybe vector for the number of floats -> this would be more generic and possibly work for 1-n number of float control.
void UiControl::RenderVec4PropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties /* = {} */)
{
    Math::Vec4 value;
    ReflectType::SetValueFromString(value, property.GetPropertyValue());

    std::string propertyName = controlProperties.propertyName;
    if (controlProperties.firstLetterUppercase)
    {
        propertyName[0] = static_cast<char>(std::toupper(propertyName[0]));
    }

    const std::string propertyId = controlProperties.propertyName + controlProperties.propertyId;
    const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, propertyName.c_str());

    // Todo Christian: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + lineHeight - ImGui::CalcTextSize(propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", propertyName.c_str());

    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(4, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 4.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);
    UiControlButtonProperties buttonProperties{ .buttonSize = {lineHeight + 3.0f, lineHeight} };

    buttonProperties.buttonColor        = ImVec4{ 0.8f, 0.1f, 0.15f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.9f, 0.2f, 0.2f, 1.0f };
    RenderFloatPropertyControl("X", propertyId, value.x, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.2f, 0.7f, 0.2f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.3f, 0.8f, 0.3f, 1.0f };
    RenderFloatPropertyControl("Y", propertyId, value.y, controlProperties, buttonProperties);
    ImGui::SameLine();

    buttonProperties.buttonColor        = ImVec4{ 0.1f, 0.25f, 0.8f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.2f, 0.35f, 0.9f, 1.0f };
    RenderFloatPropertyControl("Z", propertyId, value.z, controlProperties, buttonProperties);

    ImGui::SameLine();
    buttonProperties.buttonColor        = ImVec4{ 0.6f, 0.6f, 0.65f, 1.0f };
    buttonProperties.buttonHoveredColor = ImVec4{ 0.7f, 0.7f, 0.75f, 1.0f };
    RenderFloatPropertyControl("W", propertyId, value.z, controlProperties, buttonProperties);

    ImGui::PopStyleVar(2);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

    property.SetPropertyValue(ReflectType::GetStringFromValue(value));
}

void UiControl::RenderFloatPropertyControl(const std::string& label, const std::string& propertyId, float& value, const UiControlProperties& controlProperties, const UiControlButtonProperties& buttonProperties)
{
    ImGui::PushStyleColor(ImGuiCol_Button, buttonProperties.buttonColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, buttonProperties.buttonHoveredColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, buttonProperties.buttonColor);
    if (ImGui::Button((label + "##" + propertyId).c_str(), buttonProperties.buttonSize))
    {
        value = 0.0f;
    }
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat(("##" + label + propertyId).c_str(), &value, controlProperties.dragSpeed);
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
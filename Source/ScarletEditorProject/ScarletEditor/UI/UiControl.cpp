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

void UiControl::RenderFloatPropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties /* = {} */)
{
    float value;
    ReflectType::SetValueFromString(value, property.GetPropertyValue());

    float lineHeight;
    std::string propertyId;
    RenderTwoColumnPropertyControl(lineHeight, propertyId, controlProperties);

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{ 0.0f, 4.0f });
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 4);

    ImGui::DragFloat(("##Value" + propertyId).c_str(), &value, controlProperties.dragSpeed);

    ImGui::PopStyleVar(2);
    ImGui::PopItemWidth();
    ImGui::Columns(1);

    property.SetPropertyValue(ReflectType::GetStringFromValue(value));
}

void UiControl::RenderAngle(const ScarlEnt::Property& property, const UiControlProperties& controlProperties)
{
    Math::Quat value;
    ReflectType::SetValueFromString(value, property.GetPropertyValue());

    double yaw, pitch, roll;
    value.GetYawPitchRoll(yaw, pitch, roll);

    Math::Vec3 angle{ Math::Degrees(roll), Math::Degrees(pitch), Math::Degrees(yaw) };

    float lineHeight;
    std::string propertyId;
    RenderTwoColumnPropertyControl(lineHeight, propertyId, controlProperties);

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

    float lineHeight;
    std::string propertyId;
    RenderTwoColumnPropertyControl(lineHeight, propertyId, controlProperties);

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

    float lineHeight;
    std::string propertyId;
    RenderTwoColumnPropertyControl(lineHeight, propertyId, controlProperties);

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

void UiControl::RenderAssetPropertyControl(const ScarlEnt::Property& property, const UiControlProperties& controlProperties)
{
    WeakHandle<Resource::ILazyLoadAsset> value;
    ReflectType::SetValueFromString(value, property.GetPropertyValue());

    float lineHeight;
    std::string propertyId;
    RenderTwoColumnPropertyControl(lineHeight, propertyId, controlProperties);

    const auto& loadedAssets = Engine::Instance().GetAssetManager().GetLoadedAssets(value->GetAssetType());

    const float windowWidth     = ImGui::GetWindowSize().x;
    constexpr float buttonWidth = 200.0f;

    ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

    if (ImGui::Button((value->GetAssetPath().GetFileName() + "##" + propertyId).c_str(), { buttonWidth, 25.0f }))
    {
        ImGui::OpenPopup(std::string{ "AssetSelector" + propertyId}.c_str());

        constexpr float yPadding = 5.0f;
        const ImVec2 buttonPos   = ImGui::GetItemRectMin();
        const ImVec2 buttonSize  = ImGui::GetItemRectSize();
        ImGui::SetNextWindowPos({ buttonPos.x + buttonSize.x * 0.5f, buttonPos.y + buttonSize.y + yPadding }, ImGuiCond_Always, { 0.5f, 0.0f });
    }

    if (ImGui::BeginPopup(std::string{ "AssetSelector" + propertyId }.c_str(), ImGuiWindowFlags_NoMove))
    {
        constexpr float popupMinWidth  = 250.0f;
        constexpr float popupMaxHeight = 200.0f;

        ImGui::BeginChild("PopupItems", { popupMinWidth, popupMaxHeight }, true, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto& [ulid, asset] : loadedAssets)
        {
            const std::string compName = std::string{ asset->GetAssetPath().GetFileName() };

            if (ImGui::MenuItem(compName.c_str()))
            {
                value = WeakHandle{ asset };
                property.SetPropertyValue(ReflectType::GetStringFromValue(value));
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndChild();
        ImGui::EndPopup();
    }

    ImGui::Columns(1);
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

void UiControl::RenderTwoColumnPropertyControl(float& outLineHeight, std::string& outPropertyId, const UiControlProperties& controlProperties)
{
    std::string propertyName = controlProperties.propertyName;
    if (controlProperties.firstLetterUppercase)
    {
        propertyName[0] = static_cast<char>(std::toupper(propertyName[0]));
    }

    outPropertyId = controlProperties.propertyName + controlProperties.propertyId;
    outLineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;

    ImGui::Columns(2, propertyName.c_str());

    // Todo Christian: Let this width be draggable/adjustable.
    ImGui::SetColumnWidth(0, 100.0f);

    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + outLineHeight - ImGui::CalcTextSize(propertyName.c_str()).y - ImGui::GetScrollX() - 2 * ImGui::GetStyle().ItemSpacing.y + 5.0f);
    ImGui::Text("%s", propertyName.c_str());

    ImGui::NextColumn();

}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
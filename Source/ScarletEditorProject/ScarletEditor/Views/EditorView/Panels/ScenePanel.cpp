#include "ScenePanel.h"

#ifdef DEV_CONFIGURATION

#include <format>

#include <imgui/imgui.h>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>
#include <ScarlEnt/RTTI/ComponentView.h>

#include <ScarletEngine/Components/Transform.h>

#include "Views/EditorView/View/EditorView.h"

namespace Scarlet::Editor
{

void ScenePanel::Render()
{
    auto scene = ScarlEnt::Registry::Instance().GetActiveScene();

    // ----------- Title Bar ----------------
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

    const std::string sceneFriendlyName{ scene->GetFriendlyName() };
    ImGui::Text(sceneFriendlyName.c_str());
    ImGui::SameLine();
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 35.0f, ImGui::GetCursorPosY() - 5.0f });

    if (ImGui::Button("+"))
    {
        ImGui::OpenPopup("AddEntity");
    }

    if (ImGui::BeginPopup("AddEntity")) {
        ImGui::Text("Add Entity");
        ImGui::Separator();

        if (ImGui::MenuItem("Mutable Entity"))
        {
            auto ent = scene->AddMutableEntity();
            ent.AddComponent<Component::Transform>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::EndGroup();

    ImGui::Separator();

    // ----------- Entities Tree. ----------------
    if (scene.IsValid())
    {
        if (ImGui::CollapsingHeader("Immutable Entities", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& vec = scene->GetEntityHandles();
            for (size_t i{ 0 }; i < vec.size(); ++i)
            {
                for (const ScarlEnt::ComponentView& view : vec[i]->GetComponentViews())
                {
                    for (auto& [propertyName, property] : *view.GetProperties())
                    {
                        SCARLET_DEBUG("Immutable: {}| {} : {}", propertyName, property.GetTypeAsString(), property.GetPropertyValue());
                    }
                }

                if (ImGui::TreeNodeEx(std::format("Entity {}", i).c_str(), ImGuiTreeNodeFlags_Leaf))
                {
                    ImGui::TreePop();
                }
            }
        }

        if (ImGui::CollapsingHeader("Mutable Entities", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& vec = scene->GetMutableEntityHandles();
            for (size_t i{ 0 }; i < vec.size(); ++i)
            {
                for (const ScarlEnt::ComponentView& view : vec[i]->GetComponentViews())
                {
                    for (auto& [propertyName, property] : *view.GetProperties())
                    {
                        SCARLET_DEBUG("Mutable: {}| {} : {}", propertyName, property.GetTypeAsString(), property.GetPropertyValue());
                    }
                }

                if (ImGui::TreeNodeEx(std::format("Entity {}##mutable", i).c_str(), ImGuiTreeNodeFlags_Leaf))
                {
                    ImGui::TreePop();
                }
            }
        }
    }
}

} // Namespace Scarlet::Editor

#endif // DEV_CONFIGURATION.
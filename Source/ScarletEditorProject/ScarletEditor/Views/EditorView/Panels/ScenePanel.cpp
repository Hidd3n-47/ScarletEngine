#include "ScarletEditorPch.h"
#include "ScenePanel.h"

#ifdef DEV_CONFIGURATION

#include <format>

#include <ScarlEnt/Scene.h>

#include <ScarletCoreEcs/Components/Transform.h>

#include "Core/EditorManager.h"
#include "Views/EditorView/View/EditorView.h"

namespace Scarlet::Editor
{

void ScenePanel::Render()
{
    EditorView* editorView             = dynamic_cast<EditorView*>(mView);
    SelectionManager& selectionManager = editorView->GetSelectionManager();

    WeakHandle<ScarlEnt::Scene> scene = EditorManager::Instance().GetGameScene();

    // ----------- Title Bar ----------------
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

    const std::string sceneFriendlyName{ scene->GetFriendlyName() };
    ImGui::Text("%s", sceneFriendlyName.c_str());
    ImGui::SameLine();
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 35.0f, ImGui::GetCursorPosY() - 5.0f });

    if (ImGui::Button("+"))
    {
        ImGui::OpenPopup("AddEntity");

        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
        ent.AddComponent<Component::Transform>();
    }

    ImGui::EndGroup();

    ImGui::Separator();

    // ----------- Entities Tree. ----------------
    if (scene.IsValid())
    {
        const auto& vec = scene->GetMutableEntityHandles();
        for (size_t i{ 0 }; i < vec.size(); ++i)
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
            flags |= selectionManager.GetSelectedEntity() == vec[i] ? ImGuiTreeNodeFlags_Selected : 0;
            if (ImGui::TreeNodeEx(std::format("{} {} {}", "Entity", i, "##mutable").c_str(), flags))
            {
                if (ImGui::IsItemClicked())
                {
                    selectionManager.SetSelectedEntity(vec[i]);
                }
                ImGui::TreePop();
            }
        }
    }
}

} // Namespace Scarlet::Editor

#endif // DEV_CONFIGURATION.
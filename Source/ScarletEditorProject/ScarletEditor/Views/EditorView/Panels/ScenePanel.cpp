#include "ScarletEditorPch.h"
#include "ScenePanel.h"

#ifdef DEV_CONFIGURATION

#include <format>

#include <ScarlEnt/Scene.h>

#include <ScarletCoreEcs/Components/Transform.h>
#include <ScarletCoreEcs/Components/EditorInfo.h>

#include "Core/EditorManager.h"
#include "Views/EditorView/View/EditorView.h"

namespace Scarlet::Editor
{

void ScenePanel::Render()
{
    WeakHandle<ScarlEnt::Scene> scene = EditorManager::Instance().GetGameScene();

    RenderPanelTitleBar(scene);
    RenderEntityTree(scene);
}

void ScenePanel::RenderPanelTitleBar(WeakHandle<ScarlEnt::Scene> scene)
{
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
        ent.AddComponent<Component::EditorInfo>();
        ent.AddComponent<Component::Transform>();
    }

    ImGui::EndGroup();

    ImGui::Separator();
}

void ScenePanel::RenderEntityTree(WeakHandle<ScarlEnt::Scene> scene) const
{
    EditorView* editorView             = dynamic_cast<EditorView*>(mView);
    SelectionManager& selectionManager = editorView->GetSelectionManager();

    if (scene.IsValid())
    {
        for (ScarlEnt::IEntityHandle* entityHandle : scene->GetMutableEntityHandles())
        {
            ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_Leaf;
            flags |= selectionManager.GetSelectedEntity() == entityHandle ? ImGuiTreeNodeFlags_Selected : 0;

            const std::string entityName = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(entityHandle)->GetComponent<Component::EditorInfo>().name;

            if (ImGui::TreeNodeEx(std::format("{} {}", entityName, "##mutable").c_str(), flags))
            {
                if (ImGui::IsItemClicked())
                {
                    selectionManager.SetSelectedEntity(entityHandle);
                }
                ImGui::TreePop();
            }
        }
    }
}

} // Namespace Scarlet::Editor

#endif // DEV_CONFIGURATION.
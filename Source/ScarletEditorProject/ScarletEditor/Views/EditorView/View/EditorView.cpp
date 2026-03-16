#include "ScarletEditorPch.h"
#include "EditorView.h"

#ifdef DEV_CONFIGURATION

#include <ScarletEngine/Core/Engine.h>

#include "Core/FileDialog.h"
#include "Core/EditorManager.h"
#include "Views/EditorView/Panels/ScenePanel.h"
#include "Views/EditorView/Panels/ConsolePanel.h"
#include "Views/EditorView/Panels/ViewportPanel.h"
#include "Views/EditorView/Panels/PropertiesPanel.h"

namespace
{

template <typename T>
void AddMenuItemOption(Scarlet::Editor::EditorView* view, const char* label, bool& enabled)
{
    if (ImGui::MenuItem(label, nullptr, enabled))
    {
        enabled = !enabled;

        if (enabled)
        {
            view->AddPanel<T>();
        }
        else
        {
            view->RemovePanel<T>();
        }
    }
}

} // Anonymouse Namespace.

namespace Scarlet::Editor
{

EditorView::EditorView()
{
    AddPanel<ScenePanel>();
    AddPanel<ConsolePanel>();
    AddPanel<ViewportPanel>();
    AddPanel<PropertiesPanel>();
}

void EditorView::RenderMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..."))
            {
                const std::string filepath = FileDialog::OpenFile("Scarlet Scene (*.scarlet_scene)\0*.scarlet_scene\0");

                if (!filepath.empty())
                {
                    EditorManager::Instance().OpenScene(filepath);
                }
            }

            if (ImGui::MenuItem("Save as..."))
            {
                const std::string filepath = FileDialog::SaveAsFile("Scarlet Scene (*.scarlet_scene)\0*.scarlet_scene\0");

                if (!filepath.empty())
                {
                    EditorManager::Instance().SaveScene(filepath);
                }
            }

            if (ImGui::MenuItem("Settings")) {}
            ImGui::Separator();

            if (ImGui::MenuItem("Close")) { Engine::Instance().CloseEngine(); }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            AddMenuItemOption<ConsolePanel>(this, "Console", mConsolePanel);
            AddMenuItemOption<PropertiesPanel>(this, "Properties", mPropertiesPanel);
            AddMenuItemOption<ScenePanel>(this, "Scene", mScenePanel);
            AddMenuItemOption<ViewportPanel>(this, "Viewport", mViewportPanel);

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }
}

} // Scarlet::Editor.

#endif // DEV_CONFIGURATION.
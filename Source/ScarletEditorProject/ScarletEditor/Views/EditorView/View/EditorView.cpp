#include "ScarletEditorPch.h"
#include "EditorView.h"

#ifdef DEV_CONFIGURATION

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>

#include "Core/FileDialog.h"
#include "Core/EditorManager.h"
#include "Views/EditorView/Panels/ScenePanel.h"
#include "Views/EditorView/Panels/ConsolePanel.h"
#include "Views/EditorView/Panels/ViewportPanel.h"
#include "Views/EditorView/Panels/PropertiesPanel.h"
#include "Views/EditorView/Panels/AssetBrowserPanel.h"

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
    AddPanel<AssetBrowserPanel>();

    WeakHandle<Window> window = Engine::Instance().GetMainWindow();
    window->SetSize(1920, 1080);
    window->SetPosition(-1, -1);
    window->SetTitle("Scarlet Editor");
}

void EditorView::RenderMenuBar()
{
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Open..."))
            {
                const std::string filepath = FileDialog::OpenFile(FILE_FILTER("Scarlet Scene", ".scarlet_scene"));

                if (!filepath.empty())
                {
                    EditorManager::Instance().OpenScene(filepath);
                }
            }

            if (ImGui::MenuItem("Save"))
            {
                EditorManager::Instance().SaveCurrentScene();
            }

            if (ImGui::MenuItem("Save as..."))
            {
                const std::string filepath = FileDialog::SaveAsFile(FILE_FILTER("Scarlet Scene", ".scarlet_scene"));

                if (!filepath.empty())
                {
                    EditorManager::Instance().SaveSceneAs(filepath);
                }
            }

            if (ImGui::MenuItem("Settings")) {}
            ImGui::Separator();

            if (ImGui::MenuItem("Close")) { Engine::Instance().CloseEngine(); }

            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Window"))
        {
            AddMenuItemOption<AssetBrowserPanel>(this, "AssetBrowser", mAssetBrowserPanel);
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
#include "ScarletEditorPch.h"
#include "RecentProjectsPanel.h"

#include <fstream>

#include <ScarletCore/Xml/XmlDocument.h>
#include <ScarletCore/Xml/XmlSerializer.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>

#include "Core/EditorManager.h"

#ifdef DEV_CONFIGURATION

namespace Scarlet::Editor
{
RecentProjectsPanel::RecentProjectsPanel(IView* view) 
    : Panel{ view,
        {
            .title     = "Recent Projects",
            .width     = 300.0f,
            .height    = 500.0f,
            .moveable  = false,
            .resizable = false,
            .titleBar  = false 
        } 
    }
{
    const XmlDocument document = XmlSerializer::Deserialize(GetRecentProjectFilepath());
    if (document.IsValidDocument()) [[ likely]]
    {
        for (const XmlElement& element : document.GetRootElement()->GetChildElements())
        {
            // Only store the recent project if it's still valid.
            if (std::filesystem::exists( element.GetValue()))
            {
                mRecentProjects.emplace_back(element.GetValue());
            }
        }
    }
}

RecentProjectsPanel::~RecentProjectsPanel()
{
    XmlElement* element = new XmlElement{ "RecentProjects" };

    const int size = static_cast<int>(mRecentProjects.size());
    for (int i = 0; i < std::min(size, 10); ++i)
    {
        element->AddChild("Recent", "", mRecentProjects[i]);
    }

    const std::string path = GetRecentProjectFilepath();

    if (!std::filesystem::exists(path)) [[unlikely]]
    {
        std::filesystem::create_directory(std::filesystem::path{ path }.parent_path());
    }

    const XmlDocument doc{ element };
    XmlSerializer::Serialize(doc, path);
}

void RecentProjectsPanel::Render()
{
    // Update the properties of the panel to ensure that it's rendered at the correct height.
    mProperties.height = static_cast<float>(Engine::Instance().GetMainWindow()->GetHeight());

    ImGui::Text("Recent Projects");
    ImGui::Separator();

    for (const std::string& recentProject: mRecentProjects)
    {
        std::filesystem::path path{ recentProject };
        
        if (ImGui::Button(path.filename().string().c_str()))
        {
            EditorManager::Instance().OpenGameProject(path.string());
        }
    }
}

std::string RecentProjectsPanel::GetRecentProjectFilepath()
{
    std::string path{};

    char* pValue;
    size_t len;
    if (!_dupenv_s(&pValue, &len, "APPDATA")) [[likely]]
    {
        path = std::string{ pValue } + "/Scarlet/RecentProjects.scarlet";
    }
    free(pValue);

    return path;
}

} // Namespace Scarlet::Editor

#endif // DEV_CONFIGURATION.
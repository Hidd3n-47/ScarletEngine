#include "ScarletEditorPch.h"
#include "NewProjectPanel.h"

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>

#include "Core/FileDialog.h"
#include "Core/EditorManager.h"

#include "RecentProjectsPanel.h"

#include "Views/ProjectSelectView/View/ProjectSelectView.h"

#ifdef DEV_CONFIGURATION

namespace Scarlet::Editor
{

NewProjectPanel::NewProjectPanel(IView* view)
    : Panel{ view,
        {
            .title     = "New Project",
            .width     = 500.0f,
            .height    = 500.0f,
            .positionX = 300.0f,
            .positionY = 0.0f,
            .moveable  = false,
            .resizable = false,
            .titleBar  = false 
        } 
    }
{
    // Empty.
}

void NewProjectPanel::Render()
{
    // Update the properties of the panel to ensure that it's rendered at the correct height.
    mProperties.height = static_cast<float>(Engine::Instance().GetMainWindow()->GetHeight());

    ImGui::Text("Create Project");
    ImGui::Separator();

    RenderProjectName();

    RenderSceneName();

    RenderFolderPicker();
    

    ImGui::SetCursorPos(ImVec2{ mProperties.width - 80.0f, mProperties.height - 40.0f });


    if (mProjectNameContainsSpace || mSceneNameContainsSpace)
    {
        ImGui::PushItemFlag(ImGuiItemFlags_Disabled, true);
    }
    else
    {
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{ 0.0f, 0.43f, 0.89f, 1.0f });
    }

    if (ImGui::Button("Create") && !mNewProjName.empty() && !mNewProjPath.empty())
    {
        Create();
    }

    if (mProjectNameContainsSpace || mSceneNameContainsSpace)
    {
        ImGui::PopItemFlag();
    }
    else
    {
        ImGui::PopStyleColor();
    }

}

void NewProjectPanel::RenderProjectName()
{
    const float middleY = mProperties.width * 0.5f;

    constexpr uint32 assetNameLength  = 50;
    char projectName[assetNameLength] = { };

    if (!mNewProjName.empty())
    {
        strcpy_s(projectName, mNewProjName.c_str());
        projectName[assetNameLength - 1] = '\0';
    }

    ImGui::SetCursorPosY(middleY - 100.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Project Name: ");
    ImGui::SameLine(130.0f);

    if (ImGui::InputText("##ProjectName", projectName, assetNameLength))
    {
        mNewProjName = std::string{ projectName };

        mProjectNameContainsSpace = !mNewProjName.empty() && mNewProjName.find(' ') != std::string::npos;
    }

    if (mProjectNameContainsSpace)
    {
        ImGui::SetCursorPos(ImVec2{ 150.0f, middleY - 60.0f });

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        ImGui::Text("Project name cannot contain spaces.");

        ImGui::PopStyleColor();
    }
}

void NewProjectPanel::RenderSceneName()
{
    const float middleY = mProperties.width * 0.5f;

    constexpr uint32 assetNameLength = 50;
    char sceneName[assetNameLength]  = { };

    if (!mNewSceneName.empty())
    {
        strcpy_s(sceneName, mNewSceneName.c_str());
        sceneName[assetNameLength - 1] = '\0';
    }

    ImGui::SetCursorPosY(middleY - 30.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Scene Name: ");
    ImGui::SameLine(130.0f);

    if (ImGui::InputText("##SceneName", sceneName, assetNameLength))
    {
        mNewSceneName = std::string{ sceneName };

        mSceneNameContainsSpace = !mNewSceneName.empty() && mNewSceneName.find(' ') != std::string::npos;
    }

    if (mSceneNameContainsSpace)
    {
        ImGui::SetCursorPos(ImVec2{ 150.0f, middleY - 0.0f });

        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4{ 1.0f, 0.0f, 0.0f, 1.0f });

        ImGui::Text("Scene name cannot contain spaces.");

        ImGui::PopStyleColor();
    }
}

void NewProjectPanel::RenderFolderPicker()
{
    const float middleY = mProperties.width * 0.5f;

    ImGui::SetCursorPosY(middleY + 50.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Path: ");
    ImGui::SameLine(130.0f);

    constexpr uint32 maxCharactersDisplayed = 30;

    const size_t pathCharacters = mNewProjPath.length();
    const std::string pathCapped = pathCharacters >= maxCharactersDisplayed
        ? mNewProjPath.substr(0, 3) + "..." + mNewProjPath.substr(pathCharacters - maxCharactersDisplayed)
        : mNewProjPath;
    ImGui::Text("%s", pathCapped.c_str());

    std::filesystem::path assetPath{};

    ImGui::SameLine(mProperties.width - 70.0f);

    if (ImGui::Button("Browse"))
    {
        const std::string item = FileDialog::OpenFolder();

        mNewProjPath = item;
    }
}

void NewProjectPanel::Create()
{
    const std::filesystem::path basePath{ mNewProjPath + "/" + mNewProjName };

    // First thing is to add this to recent projects.
    reinterpret_cast<RecentProjectsPanel*>(mView->GetPanel<RecentProjectsPanel>().get())->AddRecentProject(std::format("{}/{}.scarlet_project", basePath.string(), mNewProjName));

    // ============================ Create Directories:
    // ProjectName
    //     |-> Components
    //     |-> Content
    //     |-----> Scenes
    //     |-> Core
    //     |-----> Build
    //     |-----> Scenes

    const std::string relativePathsForDirectories[] = {
        "Components",
        "Content/Scenes",
        "Core/Build",
        "Core/Scenes"
    };

    for (const std::string& dir : relativePathsForDirectories)
    {
        const std::filesystem::path fullPath = basePath / dir;

        std::filesystem::create_directories(fullPath);
    }

    // ============================ Create C++ boiler plate:
    const std::string path = basePath.generic_string() + "/";
    const std::string engineAssetPath = Filepath{ FilepathDirectory::ENGINE, "" }.GetAbsolutePath();

    // The build folder is in the directory before the Engine assets.
    system(std::format(R"(python {}/../Build/ProjectCreation/CreateProject.py "{}" "{}" "{}")", engineAssetPath, mNewProjName, path, mNewSceneName).c_str());

    // ============================ Call premake5.exe to generate the game solution.
    system(std::format("{}/../Deps/Premake/premake5.exe --file=\"{}/premake5.lua\" vs2026", engineAssetPath, path).c_str());

    mNewSceneName = SCENE_NAME_DEFAULT;

    EditorManager::Instance().OpenGameProject(path + mNewProjName + ".scarlet_project");
}

} // Namespace Scarlet::Editor

#endif // DEV_CONFIGURATION.
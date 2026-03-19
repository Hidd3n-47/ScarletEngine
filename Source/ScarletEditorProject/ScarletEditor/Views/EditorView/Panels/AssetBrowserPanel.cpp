#include "ScarletEditorPch.h"
#include "AssetBrowserPanel.h"

#ifdef DEV_CONFIGURATION

#include <algorithm>

#include <ScarletCore/AssetRef.h>

#include <ScarletEngine/Rendering/Texture.h>
#include <ScarletEngine/Rendering/Renderer.h>

#include <ScarletEngine/Core/Engine.h>

#include <ScarletEngine/AssetLoading/AssetManager.h>

#include "Core/EditorManager.h"


namespace Scarlet::Editor
{

namespace
{

enum FileTypeEnum
{
    ASSET,
    SCENE,
    COMPONENT,
};

struct FileTypes
{
    uint32           iconId;
    FileTypeEnum     type;
    std::string_view extension;
};

struct FileInfo
{
    uint32       iconId;
    FileTypeEnum type;
    std::string  fileName;
};

} // Anonymous Namespace.

AssetBrowserPanel::AssetBrowserPanel(IView* view) : Panel{ view, {.title = "Asset Browser" } }
{
    mCurrentDirectory = Filepath{ FilepathDirectory::PROJECT, "" };
}

void AssetBrowserPanel::Render()
{
    AssetManager& assetManager = Engine::Instance().GetAssetManager();
    ResourceManager<Resource::Texture>& textureManager = Renderer::Instance().GetTextureManager();

    const uint32 backIconId      = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::BACK_ICON_ULID     )->GetRuntimeId())->GetId();
    const uint32 directoryIconId = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::DIRECTORY_ICON_ULID)->GetRuntimeId())->GetId();
    const uint32 assetIconId     = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::ASSET_ICON_ULID    )->GetRuntimeId())->GetId();
    const uint32 sceneIconId     = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::SCENE_ICON_ULID    )->GetRuntimeId())->GetId();
    const uint32 componentIconId = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::COMPONENT_ICON_ULID)->GetRuntimeId())->GetId();

    RenderBackAndPathTextInput(backIconId);

    const array<FileTypes, 3> fileTypes{
        FileTypes{.iconId = assetIconId    , .type = ASSET    , .extension = ".scarlet"       },
        FileTypes{.iconId = sceneIconId    , .type = SCENE    , .extension = ".scarlet_scene" },
        FileTypes{.iconId = componentIconId, .type = COMPONENT, .extension = ".h"             }
    };

    constexpr float cellSize   = 120.0f;
    const     float panelWidth = ImGui::GetContentRegionAvail().x;

    const int columnCount = std::max(static_cast<int>(panelWidth / cellSize), 1);

    ImGui::Columns(columnCount, "AssetBrowserColumns", false);

    // Split the directories and files so that alphabetical order doesn't mix the two together.
    vector<FileInfo>              scarletFiles;
    vector<std::filesystem::path> directories;

    for (const auto& asset : std::filesystem::directory_iterator(mCurrentDirectory.GetAbsolutePath()))
    {
        if (asset.is_directory())
        {
            const std::string stem = asset.path().stem().string();
            if (stem != "Core")
            {
                directories.emplace_back(asset.path());
            }
        }
        else
        {
            const std::string extension = asset.path().extension().string();
            for (const FileTypes& type : fileTypes)
            {
                if (type.extension == extension)
                {
                    scarletFiles.emplace_back(type.iconId,type.type, asset.path().filename().string());
                }
            }
        }
    }

    constexpr float iconWidth = 80.0f;

    for (const std::filesystem::path& directoryPath : directories)
    {

        ImGui::BeginGroup();

        const std::string fileName = directoryPath.filename().string();

        if (ImGui::ImageButton(fileName.c_str(), directoryIconId, ImVec2{ iconWidth, iconWidth }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
        {
            mCurrentDirectory = mCurrentDirectory / std::filesystem::relative(std::filesystem::absolute(directoryPath), mCurrentDirectory.GetAbsolutePath());
        }

        RenderIconLabel(fileName, iconWidth);

        ImGui::EndGroup();
        ImGui::NextColumn();
    }

    for (const auto& [iconId, type, fileName] : scarletFiles)
    {
        ImGui::BeginGroup();

        if (ImGui::ImageButton(fileName.c_str(), iconId, ImVec2{ iconWidth, iconWidth }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
        {
            if (type == SCENE)
            {
                EditorManager::Instance().SaveCurrentScene();
                EditorManager::Instance().OpenScene(mCurrentDirectory.GetAbsolutePath() + "\\" + fileName);
            }
        }

        RenderIconLabel(fileName, iconWidth);

        ImGui::EndGroup();
        ImGui::NextColumn();
    }

    ImGui::Columns(1);
}

void AssetBrowserPanel::RenderBackAndPathTextInput(const uint32 backIconId)
{
    ImGui::BeginGroup();

    constexpr float backIconSize = 15.0f;

    if (ImGui::ImageButton("Back", backIconId, ImVec2{ backIconSize, backIconSize }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }))
    {
        if (const std::string relativePathStr = mCurrentDirectory.GetRelativePath(); !relativePathStr.empty())
        {
            const std::filesystem::path relativePath = std::filesystem::path{ relativePathStr }.parent_path();

            mCurrentDirectory.SetRelativePath(relativePath);
        }
    }

    ImGui::SameLine();

    ImGui::Text("Input Path: ");
    ImGui::SameLine();

    char buffer[MAX_FILEPATH_LENGTH];
    const std::string relative = mCurrentDirectory.GetRelativePath();

    (void)std::snprintf(buffer, MAX_FILEPATH_LENGTH, "\\%s", relative.c_str());

    ImGui::SameLine();

    if (ImGui::InputText("##inputPath", buffer, MAX_FILEPATH_LENGTH, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        if (const std::filesystem::path enteredPath = std::filesystem::path{ buffer }; enteredPath.is_relative())
        {
            Filepath newPath = mCurrentDirectory;
            newPath.SetRelativePath(std::filesystem::path{ enteredPath.relative_path() });

            if (newPath.DoesPathExist())
            {
                mCurrentDirectory = newPath;
            }
        }
    }

    ImGui::EndGroup();
}

void AssetBrowserPanel::RenderIconLabel(const std::string& label, const float iconWidth)
{
    const float cursorX = ImGui::GetCursorPosX();
    constexpr float iconPadding = 20.0f;

    ImGui::PushTextWrapPos(cursorX + iconWidth + iconPadding);

    const ImVec2 textSize = ImGui::CalcTextSize(label.c_str(), nullptr, true, iconWidth);

    ImGui::SetCursorPosX(cursorX + (iconWidth + iconPadding * 0.5f - textSize.x ) * 0.5f);

    ImGui::TextWrapped("%s", label.c_str());

    ImGui::PopTextWrapPos();
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

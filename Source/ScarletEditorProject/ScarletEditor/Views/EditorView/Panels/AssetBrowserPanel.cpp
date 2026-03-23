#include "ScarletEditorPch.h"
#include "AssetBrowserPanel.h"

#ifdef DEV_CONFIGURATION

#include <algorithm>

#include <ScarletCore/Xml/XmlSerializer.h>

#include <ScarletReflect/ReflectType.h>

#include <ScarletEngine/Rendering/Texture.h>
#include <ScarletEngine/Rendering/Renderer.h>

#include <ScarletEngine/Core/Engine.h>

#include <ScarletEngine/AssetLoading/AssetManager.h>

#include "Core/FileDialog.h"
#include "Core/EditorManager.h"

namespace Scarlet::Editor
{

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

    RenderSearchBar();

    const array<FileTypes, 3> fileTypes{
        FileTypes{.iconId = assetIconId    , .type = ASSET    , .extension = ".scarlet"       },
        FileTypes{.iconId = sceneIconId    , .type = SCENE    , .extension = ".scarlet_scene" },
        FileTypes{.iconId = componentIconId, .type = COMPONENT, .extension = ".h"             }
    };

    constexpr float cellSize   = 120.0f;
    const     float panelWidth = ImGui::GetContentRegionAvail().x;

    const int columnCount = std::max(static_cast<int>(panelWidth / cellSize), 1);

    ImGui::Columns(columnCount, "AssetBrowserColumns", false);

    if (mSearchString.empty())
    {
        RenderUnsearchedAssets(fileTypes, directoryIconId);
    }
    else
    {
        RenderSearchedAssets(fileTypes);
    }

    ImGui::Columns(1);

    CreateAssetPopup();
}

void AssetBrowserPanel::RenderUnsearchedAssets(const array<FileTypes, 3>& fileTypes, const uint32 directoryIconId) const
{
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
                    scarletFiles.emplace_back(type.iconId, type.type, asset.path().filename().string());
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
}

void AssetBrowserPanel::RenderSearchedAssets(const array<FileTypes, 3>& fileTypes) const
{
    vector<FileInfo> scarletFiles;

    for (const auto& asset : std::filesystem::recursive_directory_iterator(mCurrentDirectory.GetAbsolutePath()))
    {
        if (!asset.is_directory())
        {
            const std::string extension = asset.path().extension().string();
            for (const FileTypes& type : fileTypes)
            {
                std::string filename = asset.path().filename().string();

                std::ranges::transform(filename, filename.begin(), [](const unsigned char c) { return std::tolower(c); });

                if (type.extension == extension && filename.find(mSearchString) != std::string::npos)
                {
                    scarletFiles.emplace_back(type.iconId, type.type, asset.path().filename().string());
                }
            }
        }
    }

    constexpr float iconWidth = 80.0f;

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
}

void AssetBrowserPanel::RenderBackAndPathTextInput(const uint32 backIconId)
{
    ImGui::BeginGroup();

    constexpr float backIconSize = 15.0f;

    ImGui::PushStyleColor(ImGuiCol_Button, ImU32{ 22ul << 16 | 22ul << 8ul | 24 });

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

    ImGui::SetNextItemWidth(500.0f);

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
    ImGui::PopStyleColor();

    ImGui::EndGroup();
}

void AssetBrowserPanel::RenderSearchBar()
{
    ImGui::SameLine();

    const float width = ImGui::GetWindowWidth();

    ImGui::SetCursorPosX(width - 300.0f);

    ImGui::SetNextItemWidth(250.0f);

    char buffer[MAX_FILEPATH_LENGTH];

    (void)std::snprintf(buffer, MAX_FILEPATH_LENGTH, "%s", mSearchString.c_str());
    if (ImGui::InputText("##testing", buffer, MAX_FILEPATH_LENGTH))
    {
        mSearchString = std::string{ buffer };
    }

    ImGui::SameLine();

    if (ImGui::Button("X##CancelSearch"))
    {
        mSearchString = "";
    }

    ImGui::NewLine();
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

void AssetBrowserPanel::RenderContextMenu()
{
    if (ImGui::BeginPopupContextWindow("AssetBrowserRightClick"))
    {
        mContextOpen = true;
        if (ImGui::MenuItem("New Folder"))
        {
            mCreateFolder = true;
        }
        if (ImGui::BeginMenu("Create Asset"))
        {
            for (uint32 i{ 0 }; i < static_cast<uint32>(AssetType::ASSET_TYPE_COUNT); ++i)
            {
                if (const AssetType type = static_cast<AssetType>(i); ImGui::MenuItem(AssetManager::AssetTypeToString(type).c_str()))
                {
                    mCreateFile        = true;
                    mAssetTypeToCreate = type;
                }
            }
            ImGui::EndMenu();
        }
        ImGui::EndPopup();
    }
    else
    {
        mContextOpen = false;
    }
}

void AssetBrowserPanel::CreateAssetPopup()
{
    if (mCreateFile || mCreateFolder)
    {
        ImGui::OpenPopup("CreateAsset");

        ImGui::SetNextWindowSize(ImVec2{ CREATE_ASSET_WIDTH, CREATE_ASSET_HEIGHT });

        const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Always, ImVec2{ 0.5f, 0.5f });
    }

    if (ImGui::BeginPopup("CreateAsset", ImGuiWindowFlags_NoMove))
    {
        if (mCreateFolder)
        {
            CreateFolder();
        }
        else
        {
            switch (mAssetTypeToCreate)
            {
            case AssetType::TEXTURE:
                CreateTexture();
                break;
            case AssetType::MATERIAL:
                CreateMaterial();
                break;
            case AssetType::MESH:
                CreateMesh();
                break;
            case AssetType::ASSET_TYPE_COUNT:
            default:
                SCARLET_WARN("Failed to create specific asset due to an unprocessed asset type.");
                mCreateFile   = false;
                mCreateFolder = false;
                break;
            }
        }

        ImGui::EndPopup();
    }
}

void AssetBrowserPanel::CreateFolder()
{
    auto ClosePopupFunc = [&] {
        mCreateFile   = false;
        mCreateFolder = false;

        ImGui::CloseCurrentPopup();
    };

    const std::string popupTitle = "Create Folder";
    const ImVec2 textSize        = ImGui::CalcTextSize(popupTitle.c_str());

    ImGui::SetCursorPosX((CREATE_ASSET_WIDTH - textSize.x) * 0.5f);
    ImGui::Text("%s", popupTitle.c_str());

    constexpr uint32 assetNameLength = 50;
    char assetName[assetNameLength]  = { };

    bool createAsset = false;

    ImGui::SetCursorPosY((CREATE_ASSET_HEIGHT - textSize.y) * 0.5f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Folder Name: ");
    ImGui::SameLine(130.0f);

    if (ImGui::InputText("##FolderName", assetName, assetNameLength, ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EditOnCreate))
    {
        createAsset = true;
    }

    ImGui::SetCursorPosY(CREATE_ASSET_HEIGHT - 35.0f);
    if (ImGui::Button("Cancel"))
    {
        ClosePopupFunc();
    }

    ImGui::SameLine(ImGui::GetWindowSize().x - 70.0f);

    if (ImGui::Button("Create") || createAsset)
    {
        if (assetName[0] != ' ')
        {
            std::filesystem::path path{ mCurrentDirectory.GetAbsolutePath() };
            path /= std::string{ assetName };

            create_directory(path);
        }

        ClosePopupFunc();
    }
}

void AssetBrowserPanel::CreateTexture()
{
    CreateMenuWithSelectingAssetFromBrowser(AssetType::TEXTURE, FILE_FILTER("Texture", ".png"));
}

void AssetBrowserPanel::CreateMaterial()
{
    static std::string enteredAssetName{};
    static Ulid selectedAssetUlid{ AssetManager::INVALID_ULID };
    static Math::Vec3 ambient{ 1.0f };
    static Math::Vec3 diffuse{ 1.0f };

    auto ClosePopupFunc = [&] {
        mCreateFile   = false;
        mCreateFolder = false;

        enteredAssetName  = "";

        selectedAssetUlid = { AssetManager::INVALID_ULID };

        ambient = Math::Vec3{ 1.0f };
        diffuse = Math::Vec3{ 1.0f };

        ImGui::CloseCurrentPopup();
        };

    const std::string popupTitle = "Create Asset";
    const ImVec2 textSize = ImGui::CalcTextSize(popupTitle.c_str());

    ImGui::SetCursorPosX((CREATE_ASSET_WIDTH - textSize.x) * 0.5f);
    ImGui::Text("%s", popupTitle.c_str());

    constexpr uint32 assetNameLength = 50;
    char assetName[assetNameLength] = { };

    if (!enteredAssetName.empty())
    {
        strcpy_s(assetName, enteredAssetName.c_str());
        assetName[assetNameLength - 1] = '\0';
    }

    const float middleY = (CREATE_ASSET_HEIGHT - textSize.y) * 0.5f;

    ImGui::SetCursorPosY(50.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Name: ");
    ImGui::SameLine(130.0f);

    if (ImGui::InputText("##AssetName", assetName, assetNameLength))
    {
        enteredAssetName = std::string{ assetName };
    }

    ImGui::SetCursorPosY(middleY - 30.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Texture: ");
    ImGui::SameLine(130.0f);

    auto selectedTexture = Engine::Instance().GetAssetManager().GetAsset(AssetType::TEXTURE, selectedAssetUlid);

    if (ImGui::Button(selectedTexture->GetAssetPath().GetFileName().c_str(), ImVec2{ CREATE_ASSET_WIDTH * 0.57f, 30.0f }))
    {
        ImGui::OpenPopup("AssetSelector");

        constexpr float yPadding = 5.0f;
        const ImVec2 buttonPos  = ImGui::GetItemRectMin();
        const ImVec2 buttonSize = ImGui::GetItemRectSize();
        ImGui::SetNextWindowPos({ buttonPos.x + buttonSize.x * 0.5f, buttonPos.y + buttonSize.y + yPadding }, ImGuiCond_Always, { 0.5f, 0.0f });
    }

    if (ImGui::BeginPopup("AssetSelector", ImGuiWindowFlags_NoMove))
    {
        const auto& loadedAssets = Engine::Instance().GetAssetManager().GetLoadedAssets(AssetType::TEXTURE);
        constexpr float popupMinWidth  = 250.0f;
        constexpr float popupMaxHeight = 200.0f;

        ImGui::BeginChild("PopupItems", { popupMinWidth, popupMaxHeight }, true, ImGuiWindowFlags_HorizontalScrollbar);

        for (auto& [loadedUlid, loadedAsset] : loadedAssets)
        {
            const std::string compName = std::string{ loadedAsset->GetAssetPath().GetFileName() };

            if (ImGui::MenuItem(compName.c_str()))
            {
                selectedAssetUlid = loadedAsset->GetUlid();
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::EndChild();
        ImGui::EndPopup();
    }

    ImGui::Text("Ambient Color");
    ImGui::SameLine(130.0f);
    ImGui::DragFloat3("##ambient", &ambient.x, 0.001f, 0.0f, 1.0f, "%.3f");

    ImGui::Text("Diffuse Color");
    ImGui::SameLine(130.0f);
    ImGui::DragFloat3("##diffuse", &diffuse.x, 0.001f, 0.0f, 1.0f, "%.3f");


    ImGui::SetCursorPosY(CREATE_ASSET_HEIGHT - 35.0f);
    if (ImGui::Button("Cancel"))
    {
        ClosePopupFunc();
    }

    ImGui::SameLine(ImGui::GetWindowSize().x - 70.0f);

    if (ImGui::Button("Create"))
    {
        if (!enteredAssetName.empty())
        {
            const Ulid ulid{};

            XmlElement* root = new XmlElement{ "Scarlet" };
            (void)root->AddAttribute("AssetType", std::to_string(static_cast<int>(AssetType::MATERIAL)));
            (void)root->AddAttribute("ulid", std::to_string(ulid));
            root->AddChild("texture"     , "", std::to_string(selectedAssetUlid));
            root->AddChild("ambientColor", "", ReflectType::GetStringFromValue(ambient));
            root->AddChild("diffuseColor", "", ReflectType::GetStringFromValue(diffuse));

            XmlSerializer::Serialize(XmlDocument{ root }, std::format("{}/{}.scarlet", mCurrentDirectory.GetAbsolutePath(), enteredAssetName));

            Engine::Instance().GetAssetManager().LoadAsset(AssetType::MATERIAL, mCurrentDirectory / std::format("{}.scarlet", enteredAssetName), ulid);
        }
        ClosePopupFunc();
    }
}


void AssetBrowserPanel::CreateMesh()
{
    CreateMenuWithSelectingAssetFromBrowser(AssetType::MESH, FILE_FILTER("Static Mesh", ".obj"));
}

void AssetBrowserPanel::CreateMenuWithSelectingAssetFromBrowser(const AssetType type, const char* browserFilter)
{
    static std::string enteredAssetName{};
    static std::string enteredAssetPath{};

    auto ClosePopupFunc = [&] {
        mCreateFile   = false;
        mCreateFolder = false;

        enteredAssetPath = "";
        enteredAssetName = "";

        ImGui::CloseCurrentPopup();
    };

    const std::string popupTitle = "Create Asset";
    const ImVec2 textSize        = ImGui::CalcTextSize(popupTitle.c_str());

    ImGui::SetCursorPosX((CREATE_ASSET_WIDTH - textSize.x) * 0.5f);
    ImGui::Text("%s", popupTitle.c_str());

    constexpr uint32 assetNameLength = 50;
    char assetName[assetNameLength] = { };

    if (!enteredAssetName.empty())
    {
        strcpy_s(assetName, enteredAssetName.c_str());
        assetName[assetNameLength - 1] = '\0';
    }

    const float middleY = (CREATE_ASSET_HEIGHT - textSize.y) * 0.5f;

    ImGui::SetCursorPosY(middleY - 20.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Name: ");
    ImGui::SameLine(130.0f);

    if (ImGui::InputText("##AssetName", assetName, assetNameLength))
    {
        enteredAssetName = std::string{ assetName };
    }

    ImGui::SetCursorPosY(middleY + 20.0f);
    ImGui::SetCursorPosX(20.0f);

    ImGui::Text("Path: ");
    ImGui::SameLine(130.0f);

    ImGui::Text("%s", enteredAssetPath.c_str());

    std::filesystem::path assetPath{};

    ImGui::SameLine(CREATE_ASSET_WIDTH - 70.0f);

    if (ImGui::Button("Browse"))
    {
        const std::string item = FileDialog::OpenFile(browserFilter);

        if (!item.empty())
        {
            assetPath = std::filesystem::relative(item, mCurrentDirectory.GetAbsolutePath());
            enteredAssetPath = assetPath.string();
        }
    }

    ImGui::SetCursorPosY(CREATE_ASSET_HEIGHT - 35.0f);
    if (ImGui::Button("Cancel"))
    {
        ClosePopupFunc();
    }

    ImGui::SameLine(ImGui::GetWindowSize().x - 70.0f);

    if (ImGui::Button("Create"))
    {
        if (!enteredAssetPath.empty() && !enteredAssetName.empty())
        {
            const Ulid ulid{};

            XmlElement* root = new XmlElement{ "Scarlet" };
            (void)root->AddAttribute("AssetType", std::to_string(static_cast<int>(type)));
            (void)root->AddAttribute("ulid", std::to_string(ulid));
            root->AddChild("filepath", "", enteredAssetPath);

            XmlSerializer::Serialize(XmlDocument{ root }, std::format("{}/{}.scarlet", mCurrentDirectory.GetAbsolutePath(), enteredAssetName));
            Engine::Instance().GetAssetManager().LoadAsset(type, mCurrentDirectory / enteredAssetPath, ulid);
        }
        ClosePopupFunc();
    }
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

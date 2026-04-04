#include "ScarletEditorPch.h"
#include "EditorManager.h"

#ifdef DEV_CONFIGURATION

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>

#include <ScarletMath/Trig.h>

#include <ScarletCore/Xml/XmlSerializer.h>

#include <ScarletEngine/Core/Engine.h>

#include <ScarletEngine/Rendering/Renderer.h>

#include <ScarletEngine/Core/Input/KeyCodes.h>
#include <ScarletEngine/Core/Input/InputManager.h>

#include <ScarletEngine/AssetLoading/AssetManager.h>

#include <ScarletCoreEcs/Components/Camera.h>
#include <ScarletCoreEcs/Components/Transform.h>
#include <ScarletCoreEcs/Components/DirectionLight.h>

#include "Views/EditorView/View/EditorView.h"
#include "Views/ProjectSelectView/View/ProjectSelectView.h"

#include "Views/EditorView/Panels/ViewportPanel.h"

namespace Scarlet::Editor
{

static void SetScarletDarkTheme()
{
    ImGuiStyle& style  = ImGui::GetStyle();
    ImVec4*     colors = style.Colors;

    // -------------------
    // Layout & Structure
    // -------------------
    style.WindowRounding    = 4.0f;
    style.ChildRounding     = 4.0f;
    style.FrameRounding     = 3.0f;
    style.PopupRounding     = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding      = 3.0f;
    style.TabRounding       = 3.0f;

    style.WindowBorderSize  = 1.0f;
    style.ChildBorderSize   = 1.0f;
    style.PopupBorderSize   = 1.0f;
    style.FrameBorderSize   = 0.0f;
    style.TabBorderSize     = 0.0f;

    style.WindowPadding     = ImVec2{ 10.0f, 8.0f };
    style.FramePadding      = ImVec2{  6.0f, 4.0f };
    style.ItemSpacing       = ImVec2{  8.0f, 6.0f };
    style.ItemInnerSpacing  = ImVec2{  6.0f, 4.0f };
    style.IndentSpacing     = 18.0f;

    // -------------------------
    // Neutral Structure Colors
    // -------------------------
    const ImVec4 topBar       = ImColor{ 15, 15, 17 }; // Menu bar (darkest)
    const ImVec4 sidePanel    = ImColor{ 18, 18, 20 }; // Hierarchy / Properties / Console
    const ImVec4 mainDock     = ImColor{ 22, 22, 24 }; // Central dock bg
    const ImVec4 surface      = ImColor{ 35, 35, 40 }; // Headers / frames
    const ImVec4 surfaceHover = ImColor{ 55, 55, 62 }; // Hover state
    const ImVec4 border       = ImColor{ 60, 60, 66 }; 

    // Scarlet Accent (interaction only)
    const ImVec4 scarlet       = ImColor{ 220, 20,  90 };
    const ImVec4 scarletHover  = ImColor{ 240, 45, 110 };
    const ImVec4 scarletActive = ImColor{ 180, 15,  70 };

    // -----
    // Text
    // -----
    colors[ImGuiCol_Text]         = ImColor{ 225, 225, 230 };
    colors[ImGuiCol_TextDisabled] = ImColor{ 120, 120, 130 };

    // ----------
    // Windowing
    // ----------
    colors[ImGuiCol_WindowBg]  = mainDock;
    colors[ImGuiCol_ChildBg]   = sidePanel;
    colors[ImGuiCol_PopupBg]   = sidePanel;
    colors[ImGuiCol_Border]    = border;
    colors[ImGuiCol_MenuBarBg] = topBar;

    // Title
    colors[ImGuiCol_TitleBg]          = topBar;
    colors[ImGuiCol_TitleBgActive]    = mainDock;
    colors[ImGuiCol_TitleBgCollapsed] = topBar;

    // ----------------------------------
    // Headers (Tree / CollapsingHeader)
    // ----------------------------------
    colors[ImGuiCol_Header]         = surface;
    colors[ImGuiCol_HeaderHovered]  = surfaceHover;
    colors[ImGuiCol_HeaderActive] = ImColor{ 70, 70, 80 };

    // -----
    // Tabs
    // -----
    colors[ImGuiCol_Tab]                       = ImColor{ 25, 25, 28 };
    colors[ImGuiCol_TabActive]                 = ImColor{ 40, 40, 45 };
    colors[ImGuiCol_TabHovered]                = surfaceHover;
    colors[ImGuiCol_TabUnfocused]              = mainDock;
    colors[ImGuiCol_TabUnfocusedActive]        = ImColor{ 30, 30, 35 };
    style.Colors[ImGuiCol_TabSelectedOverline] = scarlet;

    // --------------------------------------
    // Frames (inputs, viewport frame, etc.)
    // --------------------------------------
    colors[ImGuiCol_FrameBg]         = surface;
    colors[ImGuiCol_FrameBgHovered]  = surfaceHover;
    colors[ImGuiCol_FrameBgActive] = ImColor{ 70, 70, 80 };

    // --------
    // Buttons
    // --------
    colors[ImGuiCol_Button]        = surface;
    colors[ImGuiCol_ButtonHovered] = scarlet;
    colors[ImGuiCol_ButtonActive]  = scarletActive;

    // --------
    // Sliders
    // --------
    colors[ImGuiCol_SliderGrab]       = scarlet;
    colors[ImGuiCol_SliderGrabActive] = scarletHover;

    // -------------------
    // Checkboxes / Radio
    // -------------------
    colors[ImGuiCol_CheckMark] = scarlet;

    // -----------
    // Scrollbars
    // -----------
    colors[ImGuiCol_ScrollbarBg]          = sidePanel;
    colors[ImGuiCol_ScrollbarGrab]        = surface;
    colors[ImGuiCol_ScrollbarGrabHovered] = surfaceHover;
    colors[ImGuiCol_ScrollbarGrabActive]  = scarlet;

    // ------------
    // Resize grip
    // ------------
    colors[ImGuiCol_ResizeGrip]        = surface;
    colors[ImGuiCol_ResizeGripHovered] = scarlet;
    colors[ImGuiCol_ResizeGripActive]  = scarletActive;

    // --------
    // Docking
    // --------
    colors[ImGuiCol_DockingPreview] = scarlet;
    colors[ImGuiCol_DockingEmptyBg] = ImColor{ 12, 12, 14 };
}

EditorManager::EditorManager()
{
    Engine& engineInstance = Engine::Instance();

    mEditorView = new ProjectSelectView();

    engineInstance.SetEndRenderEvent([&] { EndRender(); });
    engineInstance.SetEditorPostUpdateEvent([&] { PostUpdate(); });
}

EditorManager::~EditorManager()
{
    Engine::Instance().SetEndRenderEvent(nullptr);

    if (mGameScene.IsValid())
    {
        SaveCurrentScene();
    }

    delete mEditorView;
}

void EditorManager::Init()
{
    mInstance = new EditorManager();

    ImGui::SetCurrentContext(static_cast<ImGuiContext*>(Engine::GetImGuiContext()));

    ImGuiIO& io = ImGui::GetIO();

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    const Filepath fontPath{ FilepathDirectory::ENGINE, "EngineAssets/Fonts/JetBrainsMono-Bold.ttf" };
    io.FontDefault = io.Fonts->AddFontFromFileTTF(fontPath.GetAbsolutePath().c_str(), 16.0f);

    SetScarletDarkTheme();
}

void EditorManager::Destroy()
{
    delete mInstance;
    mInstance = nullptr;
}

void EditorManager::RegisterEditorSystems()
{
    auto cameraMovementFunction = [&](Component::Transform& transform, Component::Camera& viewportCamera) {
        if (const std::shared_ptr<Panel> viewportPanel = mEditorView->GetPanel<ViewportPanel>(); !viewportPanel->IsHovered())
        {
            return;
        }

        if (!InputManager::IsMouseButtonDown(KeyCode::MOUSE_BUTTON_2))
        {
            return;
        }

        Math::Vec3 cameraHorizontalDirection = { }, cameraVerticalDirection = { }, cameraForwardDirection = { };

        if (InputManager::IsKeyDown(KeyCode::KEY_W))
        {
            cameraForwardDirection += -1.0f;
        }
        if (InputManager::IsKeyDown(KeyCode::KEY_S))
        {
            cameraForwardDirection += 1.0f;
        }
        if (InputManager::IsKeyDown(KeyCode::KEY_A))
        {
            cameraHorizontalDirection += -1.0f;
        }
        if (InputManager::IsKeyDown(KeyCode::KEY_D))
        {
            cameraHorizontalDirection += 1.0f;
        }
        if (InputManager::IsKeyDown(KeyCode::KEY_E))
        {
            cameraVerticalDirection += 1.0f;
        }
        if (InputManager::IsKeyDown(KeyCode::KEY_Q))
        {
            cameraVerticalDirection += -1.0f;
        }

        constexpr float SPEED_SCALING_FACTOR       = 0.05f;
        constexpr float SHIFT_KEY_SPEED_MULTIPLIER = 2.5f;

        const float shiftKeySpeedBoost   = InputManager::IsKeyDown(KeyCode::KEY_LEFT_SHIFT) ? SHIFT_KEY_SPEED_MULTIPLIER : 1.0f;
        const float totalSpeedMultiplier = SPEED_SCALING_FACTOR * shiftKeySpeedBoost;

        constexpr float SPEED_SCALING_FACTOR_YAW   = 0.45f;
        constexpr float SPEED_SCALING_FACTOR_PITCH = 0.35f;
        const Math::Vec2 moveDelta = InputManager::GetMouseDeltaThisFrame();

        transform.rotation.z += -moveDelta.x * SPEED_SCALING_FACTOR_YAW;
        transform.rotation.x += -moveDelta.y * SPEED_SCALING_FACTOR_PITCH;

        const Math::Mat4 rotationMatrix = Math::Trig::RotationMatrix(transform.rotation);

        const Math::Vec3 forwardVector = rotationMatrix[1];
        const Math::Vec3 rightVector   = rotationMatrix[0];
        const Math::Vec3 upVector      = rotationMatrix[2];

        transform.translation += rightVector   * cameraHorizontalDirection * totalSpeedMultiplier
                              -  forwardVector * cameraForwardDirection    * totalSpeedMultiplier
                              +  upVector      * cameraVerticalDirection   * totalSpeedMultiplier;

        // Todo [Bug 74]: Quaternion rotation with camera results in roll when only changing pitch and yaw.
        //constexpr float SPEED_SCALING_FACTOR_YAW   = 0.0040f;
        //constexpr float SPEED_SCALING_FACTOR_PITCH = 0.0025f;
        //const Math::Vec2 moveDelta = InputManager::GetMouseDeltaThisFrame();

        //const double yawDelta   = -moveDelta.x * SPEED_SCALING_FACTOR_YAW;
        //const double pitchDelta = -moveDelta.y * SPEED_SCALING_FACTOR_PITCH;

        //double yaw, pitch, roll;
        //transform.rotation.GetYawPitchRoll(yaw, pitch, roll);
        //yaw += yawDelta;
        //pitch += pitchDelta;
        //Math::Quat qYaw{ yaw, {0, 0, 1} };
        //Math::Quat qPitch{ pitch, {1, 0, 0} };

        //transform.rotation =  qPitch * qYaw;
        viewportCamera.dirty = true;
    };

    mGameScene->RegisterSystem<Component::Transform, Component::Camera>(cameraMovementFunction);

    Engine::Instance().RegisterEngineSystems(mGameScene);
}

void EditorManager::OpenScene(const std::string& filepath)
{
    const XmlDocument document = XmlSerializer::Deserialize(filepath);

    const std::string newSceneFriendlyName = std::filesystem::path{ filepath }.stem().string();

    if (const auto activeScene = ScarlEnt::Registry::Instance().GetActiveScene();
        activeScene.IsValid() && activeScene->GetFriendlyName() == newSceneFriendlyName)
    {
        SCARLET_ERROR("Failed to open scene as this scene is already currently open.");
        return;
    }

    if (EditorView* view = dynamic_cast<EditorView*>(mEditorView); view) [[likely]]
    {
        view->GetSelectionManager().SetSelectedEntity(nullptr);
    }

    mGameScene = ScarlEnt::Registry::Instance().GetOrCreateScene(newSceneFriendlyName);

    mCurrentSceneFilepath = filepath;

    for (const XmlElement& entityNode : document.GetRootElement()->GetChildElements())
    {
        if (entityNode.GetTagName() == "ProjectPath") continue;

        ScarlEnt::MutableEntityHandle entity = mGameScene->AddMutableEntity();

        for (const XmlElement& component : entityNode.GetChildElements())
        {
            auto* componentProperties = ScarlEnt::Registry::Instance().AddComponentToHandle(component.GetAttributeValue("typeId").c_str(), &entity);

            for (auto& [propertyName, property] : *componentProperties)
            {
                for (const XmlElement& componentProperty : component.GetChildElements())
                {
                    if (componentProperty.GetTagName() == propertyName)
                    {
                        property.SetPropertyValue(componentProperty.GetValue());
                    }
                }
            }
        }
    }

    CreateViewportCameraEntity();

    RegisterEditorSystems();

    if (!ScarlEnt::Registry::Instance().GetActiveScene().IsValid()) [[unlikely]]
    {
        ScarlEnt::Registry::Instance().SetActiveScene(mGameScene);
    }
    else [[likely]]
    {
        ScarlEnt::Registry::Instance().ChangeScene(mGameScene, true);
    }
}

void EditorManager::SaveSceneAs(const std::string& filepath)
{
    const std::string fileExtension         = ".scarlet_scene";
    const std::string filepathWithExtension = filepath.ends_with(fileExtension) ? filepath : filepath + fileExtension;

    XmlElement* sceneNode = new XmlElement("ScarletScene");

    const vector<ScarlEnt::IEntityHandle*>& entities = mGameScene->GetMutableEntityHandles();

    for (ScarlEnt::IEntityHandle* handle : entities)
    {
        XmlElement entityNode{ "Entity" };

        for (auto& component : handle->GetComponentViews())
        {
            std::string componentAttributeString = "typeId=\"" + component.GetComponentId().name + "\"";
            XmlElement  componentNode{ "Component", componentAttributeString };

            for (auto& [componentTag, componentProperty] : *component.GetProperties())
            {
                const std::string propertyValue   = componentProperty.GetPropertyValue();
                const std::string attributeString = "type=\"" + componentProperty.GetTypeAsString() + "\"";

                componentNode.AddChild(componentTag, attributeString, propertyValue);
            }

            entityNode.AddChild(componentNode);
        }

        sceneNode->AddChild(entityNode);
    }

    XmlSerializer::Serialize(XmlDocument{ sceneNode }, filepathWithExtension);
}

void EditorManager::SaveCurrentScene()
{
    SaveSceneAs(mCurrentSceneFilepath);
}

void EditorManager::EndRender() const
{
    mEditorView->Render();

    ImGui::Render();
}

void EditorManager::PostUpdate()
{
    if (!mProjectPathToOpen.empty())
    {
        OpenGameProject();
    }

    if (mShouldHotReloadGame)
    {
        Engine::Instance().ReloadGameDll();

        OpenScene(mCurrentSceneFilepath);

        CreateViewportCameraEntity();

        mShouldHotReloadGame = false;
     }
}

void EditorManager::OpenGameProject()
{
    SCARLET_DEBUG("Opening scarlet project: {}", mProjectPathToOpen);

    XmlDocument projectDoc = XmlSerializer::Deserialize(mProjectPathToOpen);

    const std::string relativeScenePath = projectDoc.GetRootElement()->GetChildElements()[0].GetAttributeValue("relativePath");
    
    const std::filesystem::path projectToOpen{ mProjectPathToOpen };
    const std::filesystem::path projectDirectory = projectToOpen.parent_path();
    Filepath::SetProjectDirectory(projectDirectory);
    mProjectName = projectToOpen.stem().string();

    delete mEditorView;
    mEditorView = new EditorView();

    Engine::Instance().GetAssetManager().LoadScarletAssets(Filepath{ FilepathDirectory::PROJECT, "" });

    Engine::Instance().ReloadGameDll();

    OpenScene((projectDirectory / relativeScenePath).string());

    mProjectPathToOpen = "";
}

Component::Camera& EditorManager::GetViewportCamera()
{
    return mCameraEntity.GetComponent<Component::Camera>();
}

Component::Transform& EditorManager::GetViewportCameraTransform()
{
    return mCameraEntity.GetComponent<Component::Transform>();
}

void EditorManager::CreateViewportCameraEntity()
{
    Component::Transform cameraTransform{ };
    cameraTransform.translation = Math::Vec3{ 0.0f, -10.0f, 2.0f };
    mCameraEntity = mGameScene->AddEntity<Component::Transform, Component::Camera, Component::DirectionLight>(std::move(cameraTransform), Component::Camera{}, Component::DirectionLight{ });
    mGameScene->SetCameraEntityHandle(&mCameraEntity);
}

} // Namespace Scarlet::Editor.


#endif // DEV_CONFIGURATION.

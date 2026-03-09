#include "ScarletEditorPch.h"
#include "EditorManager.h"

#ifdef DEV_CONFIGURATION

#include <format>

#include <glfw/glfw3.h>

#include <ScarlEnt/Registry.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>

#include <ScarletEngine/Components/Camera.h>
#include <ScarletEngine/Components/Transform.h>
#include <ScarletEngine/Components/DirectionLight.h>

#include "ScarlEnt/Scene.h"
#include "Views/EditorView/View/EditorView.h"

namespace Scarlet::Editor
{

static void SetScarletDarkTheme()
{
    ImGuiStyle& style = ImGui::GetStyle();
    ImVec4* colors = style.Colors;

    // ------------------------------------------------------------
    // Layout & Structure
    // ------------------------------------------------------------
    style.WindowRounding = 4.0f;
    style.ChildRounding = 4.0f;
    style.FrameRounding = 3.0f;
    style.PopupRounding = 4.0f;
    style.ScrollbarRounding = 4.0f;
    style.GrabRounding = 3.0f;
    style.TabRounding = 3.0f;

    style.WindowBorderSize = 1.0f;
    style.ChildBorderSize = 1.0f;
    style.PopupBorderSize = 1.0f;
    style.FrameBorderSize = 0.0f;
    style.TabBorderSize = 0.0f;

    style.WindowPadding = ImVec2(10, 8);
    style.FramePadding = ImVec2(6, 4);
    style.ItemSpacing = ImVec2(8, 6);
    style.ItemInnerSpacing = ImVec2(6, 4);
    style.IndentSpacing = 18.0f;

    // ------------------------------------------------------------
    // Neutral Structure Colors
    // ------------------------------------------------------------

    ImVec4 topBar = ImColor(15, 15, 17);   // Menu bar (darkest)
    ImVec4 sidePanel = ImColor(18, 18, 20);   // Hierarchy / Properties / Console
    ImVec4 mainDock = ImColor(22, 22, 24);   // Central dock bg
    ImVec4 surface = ImColor(35, 35, 40);   // Headers / frames
    ImVec4 surfaceHover = ImColor(55, 55, 62);   // Hover state
    ImVec4 border = ImColor(60, 60, 66);

    // Scarlet Accent (interaction only)
    ImVec4 scarlet = ImColor(220, 20, 90);
    ImVec4 scarletHover = ImColor(240, 45, 110);
    ImVec4 scarletActive = ImColor(180, 15, 70);

    // ------------------------------------------------------------
    // Text
    // ------------------------------------------------------------
    colors[ImGuiCol_Text] = ImColor(225, 225, 230);
    colors[ImGuiCol_TextDisabled] = ImColor(120, 120, 130);

    // ------------------------------------------------------------
    // Windowing
    // ------------------------------------------------------------
    colors[ImGuiCol_WindowBg] = mainDock;
    colors[ImGuiCol_ChildBg] = sidePanel;
    colors[ImGuiCol_PopupBg] = sidePanel;
    colors[ImGuiCol_Border] = border;
    colors[ImGuiCol_MenuBarBg] = topBar;

    // Title
    colors[ImGuiCol_TitleBg] = topBar;
    colors[ImGuiCol_TitleBgActive] = mainDock;
    colors[ImGuiCol_TitleBgCollapsed] = topBar;

    // ------------------------------------------------------------
    // Headers (Tree / CollapsingHeader)
    // ------------------------------------------------------------
    colors[ImGuiCol_Header] = surface;
    colors[ImGuiCol_HeaderHovered] = surfaceHover;
    colors[ImGuiCol_HeaderActive] = ImColor(70, 70, 80);

    // ------------------------------------------------------------
    // Tabs
    // ------------------------------------------------------------
    colors[ImGuiCol_Tab] = ImColor(25, 25, 28);
    colors[ImGuiCol_TabActive] = ImColor(40, 40, 45);
    colors[ImGuiCol_TabHovered] = surfaceHover;
    colors[ImGuiCol_TabUnfocused] = mainDock;
    colors[ImGuiCol_TabUnfocusedActive] = ImColor(30, 30, 35);
    style.Colors[ImGuiCol_TabSelectedOverline] = scarlet;

    // ------------------------------------------------------------
    // Frames (inputs, viewport frame, etc.)
    // ------------------------------------------------------------
    colors[ImGuiCol_FrameBg] = surface;
    colors[ImGuiCol_FrameBgHovered] = surfaceHover;
    colors[ImGuiCol_FrameBgActive] = ImColor(70, 70, 80);

    // ------------------------------------------------------------
    // Buttons
    // ------------------------------------------------------------
    colors[ImGuiCol_Button] = surface;
    colors[ImGuiCol_ButtonHovered] = scarlet;
    colors[ImGuiCol_ButtonActive] = scarletActive;

    // ------------------------------------------------------------
    // Sliders 
    // ------------------------------------------------------------
    colors[ImGuiCol_SliderGrab] = scarlet;
    colors[ImGuiCol_SliderGrabActive] = scarletHover;

    // ------------------------------------------------------------
    // Checkboxes / Radio
    // ------------------------------------------------------------
    colors[ImGuiCol_CheckMark] = scarlet;

    // ------------------------------------------------------------
    // Scrollbars
    // ------------------------------------------------------------
    colors[ImGuiCol_ScrollbarBg] = sidePanel;
    colors[ImGuiCol_ScrollbarGrab] = surface;
    colors[ImGuiCol_ScrollbarGrabHovered] = surfaceHover;
    colors[ImGuiCol_ScrollbarGrabActive] = scarlet;

    // ------------------------------------------------------------
    // Resize grip
    // ------------------------------------------------------------
    colors[ImGuiCol_ResizeGrip] = surface;
    colors[ImGuiCol_ResizeGripHovered] = scarlet;
    colors[ImGuiCol_ResizeGripActive] = scarletActive;

    // ------------------------------------------------------------
    // Docking
    // ------------------------------------------------------------
    colors[ImGuiCol_DockingPreview] = scarlet;
    colors[ImGuiCol_DockingEmptyBg] = ImColor(12, 12, 14);
}

EditorManager::EditorManager()
{
    Engine& engineInstance = Engine::Instance();

    mEditorView = new EditorView();

    engineInstance.SetEndRenderEvent([&] { EndRender(); });

    mEditorScene = ScarlEnt::Registry::Instance().CreateScene("EditorScene");
    mGameScene   = ScarlEnt::Registry::Instance().CreateScene("GameScene");

    Component::Transform cameraTransform{ };
    cameraTransform.position = Math::Vec3{ 0.0f, -10.0f, 2.0f };
    mCameraEntity = mEditorScene->AddEntity<Component::Transform, Component::Camera, Component::DirectionLight>(std::move(cameraTransform), Component::Camera{}, Component::DirectionLight{ });
    mEditorScene->SetCameraEntityHandle(&mCameraEntity);

    ScarlEnt::Registry::Instance().SetActiveScene(mEditorScene);
}

EditorManager::~EditorManager()
{
    Engine::Instance().SetEndRenderEvent(nullptr);

    delete mEditorView;
}

void EditorManager::Init()
{
    mInstance = new EditorManager();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Engine::Instance().GetMainWindow()->GetNativeWindow()), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    io.FontDefault = io.Fonts->AddFontFromFileTTF("E:/Programming/ScarletEngine/EngineAssets/Fonts/JetBrainsMono-Bold.ttf", 16.0f);

    SetScarletDarkTheme();
}

void EditorManager::Destroy()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    delete mInstance;
    mInstance = nullptr;
}

void EditorManager::EndRender() const
{
    mEditorView->Render();

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

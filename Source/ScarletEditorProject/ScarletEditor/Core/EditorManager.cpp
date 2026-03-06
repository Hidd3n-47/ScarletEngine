#include "EditorManager.h"

#ifdef DEV_CONFIGURATION

#include <string>
#include <format>

#include <glfw/glfw3.h>

#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include <ScarletCore/Defines.h>
#include <ScarletCore/PrimitiveTypes.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>
#include <ScarletEngine/Rendering/Framebuffer.h>

#include <ScarletEngine/Components/Transform.h>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>

namespace Scarlet::Editor
{

void SetScarletDarkTheme()
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

    WeakHandle<Window> mainWindow = engineInstance.GetMainWindow();
    mViewportWidth  = mainWindow->GetWidth();
    mViewportHeight = mainWindow->GetHeight();

    mLastFrameWidth  = mViewportWidth;
    mLastFrameHeight = mViewportHeight;

    mFramebuffer = new Framebuffer(mViewportWidth, mViewportHeight);

    engineInstance.SetBeginRenderEvent([&] { BeingRender(); });
    engineInstance.SetEndRenderEvent([&] { EndRender(); });
}

EditorManager::~EditorManager()
{
    Engine::Instance().SetBeginRenderEvent(nullptr);
    Engine::Instance().SetEndRenderEvent(nullptr);

    delete mFramebuffer;
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

void EditorManager::BeingRender() const
{
    mFramebuffer->Bind();
    glViewport(0, 0, static_cast<int>(mViewportWidth), static_cast<int>(mViewportHeight));
}

void EditorManager::EndRender()
{
    Framebuffer::Unbind();

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    bool open = true;
    ImGui::Begin("DockSpace", &open, windowFlags);

    ImGui::PopStyleVar(3);

    const ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2{ 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    }

    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Settings")) {}
            ImGui::Separator();

            if (ImGui::MenuItem("Close")) { Engine::Instance().CloseEngine(); }

            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::End(); // Dockspace.

    ImGui::Begin("Hierarchy");

    auto scene = ScarlEnt::Registry::Instance().GetActiveScene();

    // ----------- Title Bar ----------------
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

    const std::string sceneFriendlyName{ scene->GetFriendlyName() };
    ImGui::Text(sceneFriendlyName.c_str());
    ImGui::SameLine();
    ImGui::SetCursorPos({ ImGui::GetWindowWidth() - 35.0f, ImGui::GetCursorPosY() - 5.0f });

    if (ImGui::Button("+"))
    {
        ImGui::OpenPopup("AddEntity");
    }

    if (ImGui::BeginPopup("AddEntity")) {
        ImGui::Text("Add Entity");
        ImGui::Separator();

        if (ImGui::MenuItem("Mutable Entity"))
        {
            auto ent = scene->AddMutableEntity();
            ent.AddComponent<Component::Transform>();
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::EndGroup();

    ImGui::Separator();

    // ----------- Entities Tree. ----------------
    if (scene.IsValid())
    {
        if (ImGui::CollapsingHeader("Immutable Entities", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& vec = scene->GetEntityHandles();
            for (size_t i{ 0 }; i < vec.size(); ++i)
            {
                for (const ScarlEnt::ComponentView& view : vec[i]->GetComponentViews())
                {
                    for (auto& [propertyName, property] : *view.GetProperties())
                    {
                        SCARLET_DEBUG("Immutable: {}| {} : {}", propertyName, property.GetTypeAsString(), property.GetPropertyValue());
                    }
                }

                if (ImGui::TreeNodeEx(std::format("Entity {}", i).c_str(), ImGuiTreeNodeFlags_Leaf))
                {
                    ImGui::TreePop();
                }
            }
        }

        if (ImGui::CollapsingHeader("Mutable Entities", ImGuiTreeNodeFlags_DefaultOpen))
        {
            const auto& vec = scene->GetMutableEntityHandles();
            for (size_t i{ 0 }; i < vec.size(); ++i)
            {
                for (const ScarlEnt::ComponentView& view : vec[i]->GetComponentViews())
                {
                    for (auto& [propertyName, property] : *view.GetProperties())
                    {
                        SCARLET_DEBUG("Mutable: {}| {} : {}", propertyName, property.GetTypeAsString(), property.GetPropertyValue());
                    }
                }


                if (ImGui::TreeNodeEx(std::format("Entity {}##mutable", i).c_str(), ImGuiTreeNodeFlags_Leaf))
                {
                    ImGui::TreePop();
                }
            }
        }
    }

    ImGui::End(); // Hierarchy.

    ImGui::Begin("Viewport", nullptr, ImGuiWindowFlags_NoScrollbar);

    const ImVec2 size = ImGui::GetWindowSize();

    const ImVec2 viewportSize = ImGui::GetWindowSize();

    constexpr float toolbarHeight = 30.0f;

    ImGui::BeginChild("ViewportToolbar", ImVec2(viewportSize.x, toolbarHeight), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if (ImGui::Button("Translate")) {}
    ImGui::SameLine();
    if (ImGui::Button("Rotate")) {}
    ImGui::SameLine();
    if (ImGui::Button("Scale")) {}

    ImGui::SameLine(viewportSize.x - 150);
    if (ImGui::Button("Hot Reload DLL"))
    {
        Engine::Instance().ReloadGameDll();
    }

    ImGui::EndChild();

    const ImVec2 remainingSize = ImVec2(viewportSize.x, viewportSize.y - toolbarHeight);

    const uint32 width  = static_cast<uint32>(remainingSize.x);
    const uint32 height = static_cast<uint32>(remainingSize.y);

    const bool resizing = mLastFrameWidth != width || mLastFrameHeight != height;

    mLastFrameWidth  = width;
    mLastFrameHeight = height;

    if (!resizing)
    {
        ImGui::Image(mFramebuffer->GetColorAttachmentId(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (width != mViewportWidth || height != mViewportHeight)
        {
            mFramebuffer->Resize(width, height);

            mViewportWidth  = width;
            mViewportHeight = height;
        }
    }

    ImGui::End(); // Viewport.

    ImGui::Begin("Console");
    ImGui::End(); // Console.

    ImGui::Begin("Properties");

    //if (ImGui::CollapsingHeader("Camera"))
    //{
    //    ImGui::DragFloat3("Position", &mCameraPosition[0], 0.05f);

    //    float rotation[3] = { 0.0f, 0.0f, 0.0f };
    //    ImGui::DragFloat3("Rotation", rotation, 0.05f);

    //    Math::Vec3 forward = mRenderCamera.GetForwardVector();
    //    Math::Vec3 right = mRenderCamera.GetRightVector();
    //    Math::Vec3 up = mRenderCamera.GetUpVector();
    //    RotateCamera(forward, right, up, rotation[0], rotation[1], rotation[2]);
    //    mRenderCamera.SetForwardVector(forward);
    //    mRenderCamera.SetRightVector(right);
    //    mRenderCamera.SetUpVector(up);
    //}

    //if (ImGui::CollapsingHeader("Lighting"))
    //{
    //    ImGui::DragFloat("Ambient", &ambientIntensity, 0.01f, 0.0f, 1.0f);
    //    ImGui::DragFloat("Diffuse", &diffuseIntensity, 0.01f, 0.0f, 1.0f);
    //    ImGui::DragFloat("RimExponent", &rimPower, 0.1f, -5.0f, 5.0f);
    //    ImGui::DragFloat3("Direction", direction, 0.1f);
    //}

    ImGui::End(); // Properties.

    ImGui::Render();

    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backupCurrentContext);
    }
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

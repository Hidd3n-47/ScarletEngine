#include "ScarletEnginePch.h"
#include "Renderer.h"

#define GLEW_STATIC
#include <glew/glew.h>
#include <glfw/glfw3.h>

#ifdef DEV_CONFIGURATION
#include <imgui/imgui.h>
#include <imgui/imgui_internal.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include <ScarlEnt/Registry.h>

#include "Framebuffer.h"
#endif // DEV_CONFIGURATION.

#include <algorithm>

#include "Core/Window/Window.h"

#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Core/Engine.h"
#include "ScarlEnt/Scene.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "CubeMapTexture.h"

namespace
{

static constexpr float SKY_BOX_VERTICES[] =
{
    -1.0f, -1.0f,  1.0f,
     1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,
     1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f,
    -1.0f,  1.0f, -1.0f
};

static constexpr int SKY_BOX_INDEX_COUNT = 36;

static constexpr uint32 SKY_BOX_INDICES[SKY_BOX_INDEX_COUNT] =
{
    1, 2, 6,
    6, 5, 1,

    0, 4, 7,
    7, 3, 0,

    4, 5, 6,
    6, 7, 4,

    0, 3, 2,
    2, 1, 0,

    0, 1, 5,
    5, 4, 0,

    3, 7, 6,
    6, 2, 3
};

Scarlet::Math::Vec3 RotateAroundAxis(const Scarlet::Math::Vec3& v, const Scarlet::Math::Vec3& axis, float angleRad)
{
    Scarlet::Math::Vec3 k = Scarlet::Math::Normalize(axis);
    float cosA = std::cos(angleRad);
    float sinA = std::sin(angleRad);

    return v * cosA + Scarlet::Math::Cross(k, v) * sinA + k * Scarlet::Math::Dot(k, v) * (1.0f - cosA);
}

void RotateCamera(Scarlet::Math::Vec3& forward, Scarlet::Math::Vec3& right, Scarlet::Math::Vec3& up,
    float yawDegrees, float pitchDegrees, float rollDegrees)
{

    // Convert degrees to radians
    float yaw   = Scarlet::Math::Radians(yawDegrees);
    float pitch = Scarlet::Math::Radians(pitchDegrees);
    float roll  = Scarlet::Math::Radians(rollDegrees);

    // Yaw around global Z (up)
    forward = RotateAroundAxis(forward, Scarlet::Math::Vec3{ 0,0,1 }, yaw);
    right = RotateAroundAxis(right, Scarlet::Math::Vec3{ 0,0,1 }, yaw);
    up = RotateAroundAxis(up, Scarlet::Math::Vec3{ 0,0,1 }, yaw);

    // Pitch around right vector
    forward = RotateAroundAxis(forward, right, pitch);
    up = RotateAroundAxis(up, right, pitch);

    // Roll around forward vector
    right = RotateAroundAxis(right, forward, roll);
    up = RotateAroundAxis(up, forward, roll);

    // Re-orthonormalize to prevent drift
    forward = Scarlet::Math::Normalize(forward);
    right = Scarlet::Math::Normalize(Scarlet::Math::Cross(forward, up));
    up = Scarlet::Math::Normalize(Scarlet::Math::Cross(right, forward));
}

} // Anonymous namespace.

namespace Scarlet
{

#ifdef DEV_CONFIGURATION
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
#endif // DEV_CONFIGURATION.

void Renderer::InitApi(WeakHandle<Window> window)
{
    const uint32 glewOkay = glewInit();
    SCARLET_ASSERT(glewOkay == GLEW_OK && "Failed to initialise glew!");

    // Depth testing.
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    // Back Face culling.
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);

#ifdef DEV_CONFIGURATION
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetNativeWindow()), true);
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
#endif // DEV_CONFIGURATION.

    mInstance = new Renderer(window->GetWidth(), window->GetHeight());
}

void Renderer::TerminateApi()
{
    delete mInstance;
    mInstance = nullptr;

#ifdef DEV_CONFIGURATION
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif // DEV_CONFIGURATION.
}

Renderer::Renderer(const uint32 width, const uint32 height)
    : mViewportWidth(width)
    , mViewportHeight(height)
    , mLastFrameWidth(width)
    , mLastFrameHeight(height)
    , mShader("E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.frag")
    , mSkyBoxShader("E:/Programming/ScarletEngine/EngineAssets/Shaders/skybox.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/skybox.frag")
    , mInstanceBuffer(MAX_INSTANCE_COUNT * sizeof(Math::Mat4))
{
#ifdef DEV_CONFIGURATION
    mFramebuffer = new Framebuffer(width, height);
#endif // DEV_CONFIGURATION.

    mSkyBoxVao = new VertexArray();
    mSkyBoxVbo = new VertexBuffer(SKY_BOX_VERTICES, sizeof(SKY_BOX_VERTICES) * sizeof(float));
    mSkyBoxIbo = new IndexBuffer(SKY_BOX_INDICES, sizeof(SKY_BOX_INDICES) / sizeof(uint32));

    mSkyBoxVbo->PushVertexLayoutElement<float>(3);

    mSkyBoxVao->AddBuffer(*mSkyBoxVbo, false);

    mCubeMapTexture = new Resource::CubeMapTexture("E:/Programming/ScarletEngine/EngineAssets/CubeMap/");
}

Renderer::~Renderer()
{
    delete mCubeMapTexture;

    delete mSkyBoxIbo;
    delete mSkyBoxVbo;
    delete mSkyBoxVao;

#ifdef DEV_CONFIGURATION
    delete mFramebuffer;
#endif // DEV_CONFIGURATION.
}

void Renderer::AddRenderCommand(const WeakHandle<Resource::Material> material, const WeakHandle<Resource::Mesh> meshRef, const Math::Mat4& modelMatrix)
{
    const RenderGroup group{.material = material, .mesh = meshRef };

    mCommands[group].emplace_back(Math::Transpose(modelMatrix));
}

void Renderer::Render()
{
    static float ambientIntensity = 0.190f;
    static float diffuseIntensity = 0.960f;
    static float direction[3] = { -1.0f, 1.0f, -1.0f };
    static float rimPower = 3.4f;

    mRenderCamera.UpdateViewAndProjectionMatrix(mCameraPosition);

#ifdef DEV_CONFIGURATION
    mFramebuffer->Bind();
#endif // DEV_CONFIGURATION.

    glViewport(0, 0, static_cast<int>(mViewportWidth), static_cast<int>(mViewportHeight));
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.Bind();
    mShader.UploadUniform("uViewMatrix", mRenderCamera.GetViewMatrix());
    mShader.UploadUniform("uProjectionMatrix", mRenderCamera.GetProjectionMatrix());
    mShader.UploadUniform("uTexture", 0);

    mShader.UploadUniform("uCameraPosition", mCameraPosition);
    mShader.UploadUniform("uLight.color", Math::Vec3{ 1.0f, 1.0f, 1.0f });
    mShader.UploadUniform("uLight.ambientIntensity", ambientIntensity);
    mShader.UploadUniform("uLight.diffuseIntensity", diffuseIntensity);
    mShader.UploadUniform("uLight.direction", Math::Normalize(Math::Vec3{ direction[0], direction[1], direction[2] }));
    mShader.UploadUniform("uRimLightPower", rimPower);

    mInstanceBuffer.Bind();
    for (auto& [group, commands] : mCommands)
    {
        SCARLET_ASSERT(commands.size() <= MAX_INSTANCE_COUNT && "Trying to upload more meshes than the instance buffer can hold. Need to implement a flush first.");

        group.mesh->mVertexArray->Bind();

        group.material->texture->Bind();

        mShader.UploadUniform("uMaterial.ambientColor", group.material->ambientColor);
        mShader.UploadUniform("uMaterial.diffuseColor", group.material->diffuseColor);

        group.mesh->Bind();

        mInstanceBuffer.SetData(commands.data(), commands.size() * sizeof(Math::Mat4));

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(group.mesh->GetIndexCount()), GL_UNSIGNED_INT, nullptr, static_cast<int>(commands.size()));

        commands.clear();
    }

    glDepthFunc(GL_LEQUAL); 
    glDisable(GL_CULL_FACE);

    mSkyBoxShader.Bind();
    
    mSkyBoxShader.UploadUniform("uSkyBox", 0);
    mSkyBoxShader.UploadUniform("uViewMatrix", Math::Mat4(Math::Mat3(mRenderCamera.GetViewMatrix())));
    mSkyBoxShader.UploadUniform("uProjectionMatrix", mRenderCamera.GetProjectionMatrix());

    mSkyBoxVao->Bind();
    mSkyBoxVbo->Bind();
    mSkyBoxIbo->Bind();

    mCubeMapTexture->Bind();

    glDrawElements(GL_TRIANGLES, SKY_BOX_INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);

#ifdef DEV_CONFIGURATION
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

    ImGui::Begin("Hierarchy", nullptr, ImGuiDockNodeFlags_NoWindowMenuButton);

    auto scene = ScarlEnt::Registry::Instance().GetActiveScene();

    // ----------- Title Bar ----------------
    ImGui::BeginGroup();
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 5.0f);

    const std::string sceneFriendlyName { scene->GetFriendlyName() };
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
            (void)scene->AddMutableEntity();
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
                if (ImGui::TreeNodeEx(std::format("Entity {}##mutable", i).c_str(), ImGuiTreeNodeFlags_Leaf))
                {
                    ImGui::TreePop();
                }
            }
        }
    }

    ImGui::End(); // Hierarchy.

    ImGui::Begin("Viewport", nullptr, ImGuiDockNodeFlags_NoWindowMenuButton);

    const ImVec2 size   = ImGui::GetWindowSize();
    const uint32 width  = static_cast<uint32>(size.x);
    const uint32 height = static_cast<uint32>(size.y);

    const bool resizing = mLastFrameWidth != width || mLastFrameHeight != height;

    mLastFrameWidth  = width;
    mLastFrameHeight = height;

    if (!resizing)
    {
        ImGui::Image(mFramebuffer->GetColorAttachmentId(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (width != mViewportWidth || height != mViewportHeight)
        {
            mFramebuffer->Resize(width, height);

            mViewportWidth = width;
            mViewportHeight = height;
        }
    }

    ImGui::End(); // Viewport.

    ImGui::Begin("Console");
    ImGui::End(); // Console.

    ImGui::Begin("Properties");

    if (ImGui::CollapsingHeader("Camera"))
    {
        ImGui::DragFloat3("Position", &mCameraPosition[0], 0.05f);

        float rotation[3] = { 0.0f, 0.0f, 0.0f };
        ImGui::DragFloat3("Rotation", rotation, 0.05f);

        Math::Vec3 forward = mRenderCamera.GetForwardVector();
        Math::Vec3 right   = mRenderCamera.GetRightVector();
        Math::Vec3 up      = mRenderCamera.GetUpVector();
        RotateCamera(forward, right, up, rotation[0], rotation[1], rotation[2]);
        mRenderCamera.SetForwardVector(forward);
        mRenderCamera.SetRightVector(right);
        mRenderCamera.SetUpVector(up);
    }

    if (ImGui::CollapsingHeader("Lighting"))
    {
        ImGui::DragFloat("Ambient", &ambientIntensity, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("Diffuse", &diffuseIntensity, 0.01f, 0.0f, 1.0f);
        ImGui::DragFloat("RimExponent", &rimPower, 0.1f, -5.0f, 5.0f);
        ImGui::DragFloat3("Direction", direction, 0.1f);
    }

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
#endif // DEV_CONFIGURATION.
}

} // Namespace Scarlet.
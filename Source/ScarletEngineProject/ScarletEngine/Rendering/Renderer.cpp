#include "ScarletEnginePch.h"
#include "Renderer.h"

#define GLEW_STATIC
#include <glew/glew.h>
#include <glfw/glfw3.h>

#ifdef DEV_CONFIGURATION
#include <imgui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>


#include "Framebuffer.h"
#endif // DEV_CONFIGURATION.

#include "Core/Window/Window.h"

#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"
#include "Core/Engine.h"

namespace Scarlet
{

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
    , mInstanceBuffer(MAX_INSTANCE_COUNT * sizeof(Math::Mat4))
{
    mRenderCamera.UpdateViewAndProjectionMatrix({ 2.0f, -10.0f, 0.0f });

#ifdef DEV_CONFIGURATION
    mFramebuffer = new Framebuffer(width, height);
#endif // DEV_CONFIGURATION.
}

Renderer::~Renderer()
{
#ifdef DEV_CONFIGURATION
    delete mFramebuffer;
#endif // DEV_CONFIGURATION.
}

void Renderer::AddRenderCommand(const Resource::Material material, const WeakHandle<Resource::Mesh> meshRef, Math::Mat4& modelMatrix)
{
    const RenderGroup group{.material = material, .mesh = meshRef };

    mCommands[group].emplace_back((modelMatrix));
}

void Renderer::Render()
{
#ifdef DEV_CONFIGURATION
    mFramebuffer->Bind();
#endif // DEV_CONFIGURATION.
    glViewport(0, 0, static_cast<int>(mViewportWidth), static_cast<int>(mViewportHeight));
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.Bind();
    mShader.UploadUniform("uViewMatrix", mRenderCamera.GetViewMatrix());
    mShader.UploadUniform("uProjectionMatrix", mRenderCamera.GetProjectionMatrix());
    mShader.UploadUniform("uTexture", 0);

    mInstanceBuffer.Bind();
    for (auto& [group, commands] : mCommands)
    {
        SCARLET_ASSERT(commands.size() <= MAX_INSTANCE_COUNT && "Trying to upload more meshes than the instance buffer can hold. Need to implement a flush first.");

        group.mesh->mVertexArray->Bind();

        group.material.texture->Bind();
        group.mesh->Bind();

        mInstanceBuffer.SetData(commands.data(), commands.size() * sizeof(Math::Mat4));

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(group.mesh->GetIndexCount()), GL_UNSIGNED_INT, nullptr, static_cast<int>(commands.size()));

        commands.clear();
    }

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

    ImGui::Begin("Viewport");

    const ImVec2 size   = ImGui::GetWindowSize();
    const uint32 width  = static_cast<uint32>(size.x);
    const uint32 height = static_cast<uint32>(size.y);

    const bool resizing = mLastFrameWidth != width || mLastFrameHeight != height;

    mLastFrameWidth  = width;
    mLastFrameHeight = height;

    if (!resizing)
    {
        ImGui::Image(mFramebuffer->GetTextureId(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

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
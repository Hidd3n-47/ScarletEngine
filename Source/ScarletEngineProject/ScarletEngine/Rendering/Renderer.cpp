#include "ScarletEnginePch.h"
#include "Renderer.h"

#define GLEW_STATIC
#include <glew/glew.h>
#include <glfw/glfw3.h>

#include "Texture.h"
#include "Mesh.h"

#include <imgui/imgui.h>
#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include "VertexArray.h"
#include "Core/Window/Window.h"

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

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(window->GetNativeWindow()), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    ImGui::StyleColorsDark();
#endif // DEV_CONFIGURATION.

    mInstance = new Renderer();

    mInstance->mWindowWidth  = window->GetWidth();
    mInstance->mWindowHeight = window->GetHeight();
}

void Renderer::TerminateApi()
{
#ifdef DEV_CONFIGURATION
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif // DEV_CONFIGURATION.
}

Renderer::Renderer()
    : mShader("E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.frag")
    , mInstanceBuffer(MAX_INSTANCE_COUNT * sizeof(Math::Mat4))
{
    mRenderCamera.UpdateViewAndProjectionMatrix({ 2.0f, -10.0f, 2.0f });
}

void Renderer::AddRenderCommand(const Resource::Material material, const WeakHandle<Resource::Mesh> meshRef, Math::Mat4& modelMatrix)
{
    const RenderGroup group{.material = material, .mesh = meshRef };

    mCommands[group].emplace_back((modelMatrix));
}

void Renderer::Render()
{
    glViewport(0, 0, mWindowWidth, mWindowHeight);
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    mShader.Bind();
    mShader.UploadUniform("uViewMatrix", mRenderCamera.GetViewMatrix());
    mShader.UploadUniform("uProjectionMatrix", mRenderCamera.GetProjectionMatrix());
    mShader.UploadUniform("uTexture", 0);

    bool show = true;
    ImGui::ShowDemoWindow(&show);

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

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

} // Namespace Scarlet.
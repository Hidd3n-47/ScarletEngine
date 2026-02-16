#include "ScarletEnginePch.h"
#include "Renderer.h"

#define GLEW_STATIC
#include <glew/glew.h>

#include "Texture.h"
#include "Mesh.h"

#include "VertexArray.h"

namespace Scarlet
{

void Renderer::InitApi()
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

    mInstance = new Renderer();
}

Renderer::Renderer()
    : mShader("E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.frag")
    , mInstanceBuffer(100 * sizeof(Math::Mat4), 3)
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
    mShader.Bind();
    mShader.UploadUniform("uViewMatrix", mRenderCamera.GetViewMatrix());
    mShader.UploadUniform("uProjectionMatrix", mRenderCamera.GetProjectionMatrix());
    mShader.UploadUniform("uTexture", 0);

    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mInstanceBuffer.Bind();
    for (auto& [group, commands] : mCommands)
    {
        group.mesh->mVertexArray->Bind();

        group.material.texture->Bind();
        group.mesh->Bind();

        mInstanceBuffer.SetData(commands.data(), commands.size() * sizeof(Math::Mat4));

        glDrawElementsInstanced(GL_TRIANGLES, static_cast<int>(group.mesh->GetIndexCount()), GL_UNSIGNED_INT, nullptr, 100);

        commands.clear();
    }
}

} // Namespace Scarlet.
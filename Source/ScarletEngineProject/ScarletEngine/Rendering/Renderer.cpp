#include "ScarletEnginePch.h"
#include "Renderer.h"

#define GLEW_STATIC
#include <glew/glew.h>

#include <algorithm>

#include "Mesh.h"
#include "Texture.h"
#include "VertexArray.h"

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

void Renderer::TerminateApi()
{
    delete mInstance;
    mInstance = nullptr;
}

Renderer::Renderer()
    : mShader("E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.frag")
    , mSkyBoxShader("E:/Programming/ScarletEngine/EngineAssets/Shaders/skybox.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/skybox.frag")
    , mInstanceBuffer(MAX_INSTANCE_COUNT * sizeof(Math::Mat4))
{

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
}

} // Namespace Scarlet.
#include "ScarletEnginePch.h"
#include "Renderer.h"

#define GLEW_STATIC
#include <glew/glew.h>

#include <algorithm>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>

#include <ScarletMath/Trig.h>

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "VertexArray.h"

#include "IndexBuffer.h"
#include "VertexBuffer.h"
#include "CubeMapTexture.h"

#include "Components/Camera.h"
#include "Components/Transform.h"
#include "Components/DirectionLight.h"

#include "AssetLoading/ILazyLoadAsset.h"

namespace
{

constexpr float SKY_BOX_VERTICES[] =
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

constexpr int SKY_BOX_INDEX_COUNT = 36;

constexpr uint32 SKY_BOX_INDICES[SKY_BOX_INDEX_COUNT] =
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

} // Anonymous namespace.

namespace Scarlet
{

void Renderer::InitApi()
{
    const uint32 glewOkay = glewInit();
    SCARLET_ASSERT(glewOkay == GLEW_OK && "Failed to initialise glew!");

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

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
    : mShader(Filepath{ FilepathDirectory::ENGINE, "EngineAssets/Shaders/editor.vert" }, Filepath{ FilepathDirectory::ENGINE, "EngineAssets/Shaders/editor.frag" })
    , mSkyBoxShader(Filepath{ FilepathDirectory::ENGINE, "EngineAssets/Shaders/skybox.vert" }, Filepath{ FilepathDirectory::ENGINE, "EngineAssets/Shaders/skybox.frag" })
    , mInstanceBuffer(MAX_INSTANCE_COUNT * sizeof(Math::Mat4))
{
    mSkyBoxVao = new VertexArray();
    mSkyBoxVbo = new VertexBuffer(SKY_BOX_VERTICES, sizeof(SKY_BOX_VERTICES) * sizeof(float));
    mSkyBoxIbo = new IndexBuffer(SKY_BOX_INDICES, sizeof(SKY_BOX_INDICES) / sizeof(uint32));

    mSkyBoxVbo->PushVertexLayoutElement<float>(3);

    mSkyBoxVao->AddBuffer(*mSkyBoxVbo, false);

    mCubeMapTexture = new Resource::CubeMapTexture(Filepath{ FilepathDirectory::ENGINE, "EngineAssets/CubeMap/" });
}

Renderer::~Renderer()
{
    delete mCubeMapTexture;

    delete mSkyBoxIbo;
    delete mSkyBoxVbo;
    delete mSkyBoxVao;
}

void Renderer::AddRenderCommand(WeakHandle<Resource::ILazyLoadAsset> material, WeakHandle<Resource::ILazyLoadAsset> meshRef, const Math::Mat4& modelMatrix)
{
    const RenderGroup group{ .material = mMaterialManager.GetResource(material->GetRuntimeId()), .mesh = mMeshManager.GetResource(meshRef->GetRuntimeId()) };

    mCommands[group].emplace_back(modelMatrix);
}

void Renderer::Render()
{
    auto* cameraEntity = reinterpret_cast<ScarlEnt::EntityHandle<Component::Transform, Component::Camera, Component::DirectionLight>*>(ScarlEnt::Registry::Instance().GetActiveScene()->GetCameraEntityHandle());

    if (cameraEntity == nullptr) [[unlikely]]
    {
        return;
    }

    const auto& cameraTransform      = cameraEntity->GetComponent<Component::Transform>();
    const auto& cameraDirectionLight = cameraEntity->GetComponent<Component::DirectionLight>();

    Component::Camera& camera = cameraEntity->GetComponent<Component::Camera>();

    // Todo [Bug 74]: Quaternion rotation with camera results in roll when only changing pitch and yaw.
    //camera.UpdateViewAndProjectionMatrix(cameraTransform.translation, cameraTransform.rotation.GetRotationMatrix());

    camera.UpdateViewAndProjectionMatrix(cameraTransform.translation, Math::Trig::RotationMatrix(cameraTransform.rotation.z, cameraTransform.rotation.x, cameraTransform.rotation.y));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    mShader.Bind();
    mShader.UploadUniform("uViewMatrix", camera.viewMatrix);
    mShader.UploadUniform("uProjectionMatrix", camera.projectionMatrix);
    mShader.UploadUniform("uTexture", 0);

    mShader.UploadUniform("uCameraPosition", cameraTransform.translation);
    mShader.UploadUniform("uLight.color", cameraDirectionLight.lightColor);
    mShader.UploadUniform("uLight.ambientIntensity", cameraDirectionLight.ambientIntensity);
    mShader.UploadUniform("uLight.diffuseIntensity", cameraDirectionLight.diffuseIntensity);
    mShader.UploadUniform("uLight.direction", Math::Normalize(cameraDirectionLight.direction));
    mShader.UploadUniform("uRimLightPower", cameraDirectionLight.rimPower);

    mInstanceBuffer.Bind();
    for (auto& [group, commands] : mCommands)
    {
        SCARLET_ASSERT(commands.size() <= MAX_INSTANCE_COUNT && "Trying to upload more meshes than the instance buffer can hold. Need to implement a flush first.");

        group.mesh->mVertexArray->Bind();

        // Since unordered maps store a pair where the key is const, copy the render group to ensure that the material is loaded.
        // This is a trivial copy as it's only a pointer copy. This is also safe since the unordered map is only temporary and on a frame-by-frame basis,
        // so changes in the key are fine.
        WeakHandle<Resource::Material> material = group.material;

        mTextureManager.GetResource(material->texture->GetRuntimeId())->Bind();

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
    // Convert to a Mat3 to remove the translation as this shouldn't affect skymaps.
    mSkyBoxShader.UploadUniform("uViewMatrix", Math::Mat4(Math::Mat3(camera.viewMatrix)));
    mSkyBoxShader.UploadUniform("uProjectionMatrix", camera.projectionMatrix);

    mSkyBoxVao->Bind();
    mSkyBoxVbo->Bind();
    mSkyBoxIbo->Bind();

    mCubeMapTexture->Bind();

    glDrawElements(GL_TRIANGLES, SKY_BOX_INDEX_COUNT, GL_UNSIGNED_INT, nullptr);

    glEnable(GL_CULL_FACE);
    glDepthFunc(GL_LESS);
}

} // Namespace Scarlet.
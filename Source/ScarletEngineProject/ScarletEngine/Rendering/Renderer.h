#pragma once

#include <ScarletMath/Math.h>

#include "Shader.h"
#include "Material.h"
#include "InstanceBuffer.h"

#include "RenderGroup.h"

#include "AssetLoading/ResourceManager.h"

namespace Scarlet
{

class Window;
class Framebuffer;

class VertexArray;
class VertexBuffer;
class IndexBuffer;

namespace Resource
{
class Mesh;
class Texture;
class CubeMapTexture;
class ILazyLoadAsset;
}  // Namespace Resource.

/**
 * @class Renderer: A singleton class responsible for the rendering pipeline of the application. Add a rendering command to render an instance to the screen.
 */
class Renderer
{
public:
    [[nodiscard]] inline static Renderer& Instance() { return *mInstance; }

    /**
     * @brief Initialise the backend rendering API and create an instance of the renderer.
     */
    static void InitApi();

    static void TerminateApi();

    /**
     * @brief Add a rendering command to render a mesh to the screen.
     * @param material The material of the mesh.
     * @param meshRef A reference to the mesh being rendered.
     * @param modelMatrix The model matrix (i.e. world transform) for the instance.
     */
    void AddRenderCommand(const WeakHandle<Resource::Material>, WeakHandle<Resource::ILazyLoadAsset> meshRef, const Math::Mat4& modelMatrix);

    /**
     * @brief A function to go through all commands and execute the draw calls.
     */
    void Render();

    /**
     * @brief Get a reference to the instance buffer the renderer is using.
     * @return A reference to the instance buffer the renderer is using.
     */
    InstanceBuffer& GetInstanceBuffer() { return mInstanceBuffer; }

    inline static constexpr uint32 MAX_INSTANCE_COUNT{ 100 };

    [[nodiscard]] ResourceManager<Resource::Mesh>&    GetMeshManager()    { return mMeshManager; }
    [[nodiscard]] ResourceManager<Resource::Texture>& GetTextureManager() { return mTextureManager; }
private:
    Renderer();
    ~Renderer();

    inline static Renderer* mInstance = nullptr;

    Shader mShader;
    Shader mSkyBoxShader;
    InstanceBuffer mInstanceBuffer;

    VertexArray*  mSkyBoxVao;
    VertexBuffer* mSkyBoxVbo;
    IndexBuffer*  mSkyBoxIbo;

    Resource::CubeMapTexture* mCubeMapTexture;

    std::unordered_map<RenderGroup, vector<Math::Mat4>> mCommands;

    ResourceManager<Resource::Mesh> mMeshManager;
    ResourceManager<Resource::Texture> mTextureManager;
};

} // Namespace Scarlet.


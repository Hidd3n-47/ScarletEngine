#pragma once

#include <ScarletMath/Math.h>

#include "Shader.h"
#include "InstanceBuffer.h"

#include "ScarletEngine/Rendering/RenderGroup.h"

#include "ScarletEngine/AssetLoading/ResourceManager.h"

namespace Scarlet
{

namespace Resource
{
class Mesh;
class Texture;
class Material;
class CubeMapTexture;
class ILazyLoadAsset;
}  // Namespace Resource.

class Window;
class Framebuffer;

class VertexArray;
class VertexBuffer;
class IndexBuffer;

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
    void AddRenderCommand(WeakHandle<Resource::ILazyLoadAsset> material, WeakHandle<Resource::ILazyLoadAsset> meshRef, const Math::Mat4& modelMatrix);

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

    /**
     * @brief Get the \ref ResourceManager responsible for loading in the \ref Resource::Mesh.
     * @return The \ref ResourceManager responsible for loading in meshes (i.e. the \ref Resource::Mesh).
     */
    [[nodiscard]] ResourceManager<Resource::Mesh>&     GetMeshManager()    { return mMeshManager; }
    /**
     * @brief Get the \ref ResourceManager responsible for loading in the \ref Resource::Texture.
     * @return The \ref ResourceManager responsible for loading in textures (i.e. the \ref Resource::Texture).
     */
    [[nodiscard]] ResourceManager<Resource::Texture>&  GetTextureManager() { return mTextureManager; }
    /**
     * @brief Get the \ref ResourceManager responsible for loading in the \ref Resource::Material.
     * @return The \ref ResourceManager responsible for loading in materials (i.e. the \ref Resource::Material).
     */
    [[nodiscard]] ResourceManager<Resource::Material>& GetMaterialManager() { return mMaterialManager; }
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

    ResourceManager<Resource::Mesh>     mMeshManager;
    ResourceManager<Resource::Texture>  mTextureManager;
    ResourceManager<Resource::Material> mMaterialManager;
};

} // Namespace Scarlet.


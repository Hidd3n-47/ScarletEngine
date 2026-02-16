#pragma once

#include <ScarletMath/Math.h>

#include "Shader.h"
#include "Camera.h"
#include "Material.h"
#include "InstanceBuffer.h"

#include "RenderGroup.h"

namespace Scarlet
{

namespace Resource
{
class Mesh;
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

    /**
     * @brief Add a rendering command to render a mesh to the screen.
     * @param material The material of the mesh.
     * @param meshRef A reference to the mesh being rendered.
     * @param modelMatrix The model matrix (i.e. world transform) for the instance.
     */
    void AddRenderCommand(const Resource::Material material, const WeakHandle<Resource::Mesh> meshRef, Math::Mat4& modelMatrix);

    /**
     * @brief A function to go through all commands and execute the draw calls.
     */
    void Render();

    /**
     * @brief Get a reference to the instance buffer the renderer is using.
     * @return A reference to the instance buffer the renderer is using.
     */
    InstanceBuffer& GetInstanceBuffer() { return mInstanceBuffer; }
private:
    Renderer();
    inline static Renderer* mInstance = nullptr;

    std::unordered_map<RenderGroup, vector<Math::Mat4>> mCommands;

    Shader mShader;
    InstanceBuffer mInstanceBuffer;
    Camera mRenderCamera;
};

} // Namespace Scarlet.


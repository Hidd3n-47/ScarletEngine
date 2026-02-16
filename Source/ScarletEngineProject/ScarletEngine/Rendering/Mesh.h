#pragma once

#include "MeshData.h"

namespace Scarlet
{
class VertexArray;
class VertexBuffer;
class IndexBuffer;
} // Namespace Scarlet.

namespace Scarlet::Resource
{
/**
 * @class Mesh: A class representing data needed to render a given mesh.
 */
class Mesh
{
    friend class Renderer;
public:
    Mesh(const char* filepath);
    ~Mesh();

    /**
     * @brief Bind the needed resources for the graphics pipeline to be able to render the \ref Mesh. This should be called before trying to render the \ref Mesh.
     */
    void Bind() const;

    /**
     * @brief Get the index count for the \ref Mesh.
     * @return The index count for the mesh.
     */
    [[nodiscard]] size_t GetIndexCount() const { return mMeshData.indices.size(); }
private:
    MeshData      mMeshData;
    VertexArray*  mVertexArray;
    VertexBuffer* mVertexBuffer;
    IndexBuffer*  mIndexBuffer;
};

} // Namespace Scarlet::Resource.
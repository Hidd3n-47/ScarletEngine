#pragma once

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"

namespace Scarlet
{
/**
 * @class VertexArray: A class that represents an object that stores all the state needed to supply vertex data to the graphics pipeline. \nn
 * It acts as a container that holds the configuration for vertex attributes, including which Vertex Buffer Objects provide the data, the \nn
 * format of that data, and how it should be interpreted.
 */
class VertexArray
{
public:
    VertexArray();
    ~VertexArray();

    VertexArray(const VertexArray&)            = delete;
    VertexArray(VertexArray&&)                 = delete;
    VertexArray& operator=(VertexArray&&)      = delete;
    VertexArray& operator=(const VertexArray&) = delete;

    /**
     * @brief Set the \c VertexArray to contain the layout of a given vertex buffer.
     * @ref VertexBuffer
     * @ref VertexBufferLayout
     * @param vertexBuffer The vertex buffer that the array is responsible for.
     * @param vertexBufferLayout The layout of the vertex Buffer.
     */
    void AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout) const;

    /**
     * @brief Bind the array so the graphics pipeline uses this vertex array.
     */
    void Bind() const;
    /**
     * @brief Unbind the currently bound vertex array.
     */
    static void Unbind();

private:
    uint32 mId;
};

} // Namespace Scarlet.

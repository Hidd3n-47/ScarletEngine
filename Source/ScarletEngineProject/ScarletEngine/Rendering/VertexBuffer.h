#pragma once

#include "BufferLayout.h"

namespace Scarlet
{

/**
 * @class VertexBuffer: A class responsible for uploading vertices to the GPU and storing a unique \\n
 * identifier for the vertex buffer.
 */
class VertexBuffer
{
public:
    /**
     * @brief Construct a vertex buffer with a set size and pass in the data/buffer later.
     * @param size The size of the buffer in bytes.
     */
    VertexBuffer(const uint32 size);
    /**
     * @brief Construct a vertex buffer with a set size and buffer.
     * @param vertices A pointer to the buffer of vertices.
     * @param size The size of the buffer in bytes.
     */
    VertexBuffer(const void* vertices, const uint32 size);
    ~VertexBuffer();

    VertexBuffer(const VertexBuffer&)            = delete;
    VertexBuffer(VertexBuffer&&)                 = delete;
    VertexBuffer& operator=(VertexBuffer&&)      = delete;
    VertexBuffer& operator=(const VertexBuffer&) = delete;

    /**
     * @brief Bind the buffer so the graphics pipeline uses this vertex buffer.
     */
    void Bind() const;
    /**
     * @brief Unbind the currently bound vertex buffer.
     */
    static void Unbind();

    /**
     * @brief Set the data of the buffer. This will inform the graphics pipeline to update the buffer.
     * @param vertices A pointer to the buffer of vertices.
     * @param size The size of the buffer in bytes.
     */
    void SetData(const void* vertices, const uint32 size) const;

    /**
     * @brief Add a layout element for the vertex attribute location in shader.
     * @tparam T The type uploaded to the GPU.
     * @param count The count of the type being uploaded to the GPU.
     */
    template <typename T>
    inline void PushVertexLayoutElement(const uint32 count = 0)
    {
        mLayout.PushVertexLayout<T>(count);
    }

    /**
     * @brief Add a layout element for the instance attribute location in the shader.
     * @tparam T The type uploaded to the GPU.
     * @param count The count of the type being uploaded to the GPU.
     */
    template <typename T>
    inline void PushInstanceLayoutElement(const uint32 count = 0)
    {
        mLayout.PushInstanceLayout<T>(count);
    }

    /**
     * @brief Get the buffer layout of the given vertex buffer.
     * @ref BufferLayout
     * @return The buffer layout of the given vertex buffer.
     */
    [[nodiscard]] inline const BufferLayout& GetBufferLayout() const { return mLayout; }
private:
    uint32 mId;

    BufferLayout mLayout;
};

} // Namespace Scarlet.

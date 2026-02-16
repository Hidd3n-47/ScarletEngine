#pragma once

namespace Scarlet
{

/**
 * @class IndexBuffer: A class responsible for uploading the indices to the GPU and storing a unique \\n
 * identifier for the index buffer.
 */
class IndexBuffer
{
public:
    /**
     * @brief Construct an index buffer with the number of indices but pass in the indices later.
     * @param count The number of indices in the buffer.
     */
    IndexBuffer(const size_t count);
    /**
     * @brief Construct an index buffer with the indices and the number of indices.
     * @param indices A pointer to the indices buffer.
     * @param count The number of indices in the buffer.
     */
    IndexBuffer(const uint32* indices, const size_t count);
    ~IndexBuffer();

    IndexBuffer(const IndexBuffer&)            = delete;
    IndexBuffer(IndexBuffer&&)                 = delete;
    IndexBuffer& operator=(IndexBuffer&&)      = delete;
    IndexBuffer& operator=(const IndexBuffer&) = delete;

    /**
     * @brief Bind the buffer so the graphics pipeline uses this index buffer.
     */
    void Bind() const;
    /**
     * @brief Unbind the currently bound index buffer.
     */
    static void Unbind();

    /**
     * @brief Set the data of the buffer. This will inform the graphics pipeline to update the buffer.
     * @param indices A pointer to the buffer of indices.
     * @param count The number of indices.
     */
    void SetData(const uint32* indices, const uint32 count) const;
private:
    uint32 mId;
};

} // Namespace Scarlet.

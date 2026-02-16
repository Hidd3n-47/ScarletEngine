#pragma once

namespace Scarlet
{

class InstanceBuffer
{
public:
    /**
     * @brief Construct an instance buffer with a set size and pass in the data/buffer later.
     * @param maxSize The maximum size of the buffer in bytes.
     */
    InstanceBuffer(const size_t maxSize);
    ~InstanceBuffer();

    InstanceBuffer(const InstanceBuffer&)            = delete;
    InstanceBuffer(InstanceBuffer&&)                 = delete;
    InstanceBuffer& operator=(InstanceBuffer&&)      = delete;
    InstanceBuffer& operator=(const InstanceBuffer&) = delete;

    /**
     * @brief Bind the buffer so the graphics pipeline uses this instance buffer.
     */
    void Bind() const;
    /**
     * @brief Unbind the currently bound instance buffer.
     */
    static void Unbind();

    /**
     * @brief Set the data of the buffer. This will inform the graphics pipeline to update the buffer.
     * @param instances A pointer to the buffer of instances.
     * @param size The size of the buffer in bytes.
     */
    void SetData(const void* instances, const size_t size) const;

private:
    uint32 mId;
};

} // Namespace Instance.
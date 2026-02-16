#pragma once

#include <glew/glew.h>

namespace Scarlet
{

/**
 * @struct BufferElement: Information relating to the specific element.
 */
struct BufferElement
{
    uint32 count = 0;
    uint32 type  = 0;
    uint32 size  = 0;
    bool   normalised = false;
};

/**
 * @class BufferLayout: A class representing the layout of a given vertex buffer. This can be used to \nn
 * set up the vertex array and shaders to know how to process the vertex buffer.
 */
class BufferLayout
{
public:
    /**
     * @brief Add a new element to the vertex elements.
     * @tparam T The type that is being uploaded.
     * @param count The count of the specific type.
     */
    template <typename T>
    void PushVertexLayout(const uint32 count = 0);
    /**
     * @brief Add a new element to the instance elements.
     * @tparam T The type that is being uploaded.
     * @param count The count of the specific type.
     */
    template <typename T>
    void PushInstanceLayout(const uint32 count = 0);

    /**
     * @brief Get the stride between vertices, this is how many bytes to go from on vertex to another.
     * @return The stride between vertices.
     */
    [[nodiscard]] inline uint32 GetVertexStride() const { return mVertexStride; }
    /**
     * @brief Get the stride between instance data, this is how many bytes to go from on instances data to another.
     * @return The stride between instance.
     */
    [[nodiscard]] inline uint32 GetInstanceStride() const { return mInstanceStride; }

    /** @brief Get a reference to the vertex elements pushed. */
    [[nodiscard]] inline const vector<BufferElement>& GetVertexElements() const { return mVertexElements; }
    /** @brief Get a reference to the instance elements pushed. */
    [[nodiscard]] inline const vector<BufferElement>& GetInstanceElements() const { return mInstanceElements; }
private:
    uint32 mVertexStride   = 0;
    uint32 mInstanceStride = 0;
    vector<BufferElement> mVertexElements;
    vector<BufferElement> mInstanceElements;

    /**
     * @brief Convert a C++ to an OpenGL type that can be used for OpenGL calls.
     * @tparam T The C++ type.
     * @return Returns the OpenGL type the given C++ type.
     */
    template <typename T>
    inline static constexpr uint32 TypeToGlType()
    {
        static_assert(false && "Unsupported type in the layout of the Vertex Buffer.");
        return 0;
    }
};

/* ============================================================================================================================== */

template <typename T>
void BufferLayout::PushVertexLayout(const uint32 count)
{
    constexpr uint32 size = sizeof(T);
    constexpr uint32 type = TypeToGlType<T>();

    mVertexElements.emplace_back(count, type, size, false);
    mVertexStride += size * count;
}

template <typename T>
void BufferLayout::PushInstanceLayout(const uint32 count)
{
    constexpr uint32 size = sizeof(T);
    constexpr uint32 type = TypeToGlType<T>();

    mInstanceElements.emplace_back(count, type, size, false);
    mInstanceStride += size * count;
}

template <>
inline constexpr uint32 BufferLayout::TypeToGlType<float>()
{
    return GL_FLOAT;
}

template <>
inline constexpr uint32 BufferLayout::TypeToGlType<uint32>()
{
    return GL_UNSIGNED_INT;
}

} // Namespace Scarlet.
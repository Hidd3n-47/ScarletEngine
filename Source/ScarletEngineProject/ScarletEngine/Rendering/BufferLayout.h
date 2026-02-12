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
     * @brief Add a new element to the elements.
     * @tparam T The type that is being uploaded.
     * @param count The count of the specific type.
     */
    template <typename T>
    void Push(const uint32 count = 0);

    /**
     * @brief Get the stride between vertices, this is how many bytes to go from on vertex to another.
     * @return The stride between vertices.
     */
    [[nodiscard]] inline uint32 GetStride() const { return mStride; }
    /** @brief Get a reference to the elements pushed. */
    [[nodiscard]] inline const vector<BufferElement>& GetElements() const { return mElements; }
private:
    uint32 mStride = 0;
    vector<BufferElement> mElements;

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
void BufferLayout::Push(const uint32 count)
{
    constexpr uint32 size = sizeof(T);
    constexpr uint32 type = TypeToGlType<T>();

    mElements.emplace_back(count, type, size, false);
    mStride += size * count;
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
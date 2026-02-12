#include "ScarletEnginePch.h"
#include "VertexArray.h"

namespace Scarlet
{

VertexArray::VertexArray()
{
    glGenVertexArrays(1, &mId);
}

VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &mId);
}

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer) const
{
    Bind();

    vertexBuffer.Bind();

    const BufferLayout& vertexBufferLayout = vertexBuffer.GetBufferLayout();
    const vector<BufferElement>& elements  = vertexBufferLayout.GetElements();

    uint32 offset = 0;
    for (size_t i{ 0 }; i < elements.size(); ++i)
    {
        const auto& [count, type, size, normalised] = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, count, type, normalised, static_cast<int>(vertexBufferLayout.GetStride()), reinterpret_cast<const void*>(offset));
        offset += count * size;
    }
}

void VertexArray::Bind() const
{
    glBindVertexArray(mId);
}

void VertexArray::Unbind()
{
    glBindVertexArray(0);
}

} // Namespace Scarlet.
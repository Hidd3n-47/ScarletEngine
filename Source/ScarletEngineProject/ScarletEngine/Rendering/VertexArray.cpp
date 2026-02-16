#include "ScarletEnginePch.h"
#include "VertexArray.h"

#include "Renderer.h"

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

    const BufferLayout& vertexBufferLayout      = vertexBuffer.GetBufferLayout();
    const vector<BufferElement>& vertexElements = vertexBufferLayout.GetVertexElements();

    uint32 offset = 0;
    const size_t vertexSize = vertexElements.size();
    for (size_t i{ 0 }; i < vertexSize; ++i)
    {
        const auto& [count, type, size, normalised] = vertexElements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, count, type, normalised, static_cast<int>(vertexBufferLayout.GetVertexStride()), reinterpret_cast<const void*>(offset));
        offset += count * size;
    }

    Renderer::Instance().GetInstanceBuffer().Bind();

    offset = 0;
    const vector<BufferElement>& instanceElements = vertexBufferLayout.GetInstanceElements();
    for (size_t i{ 0 }; i < instanceElements.size(); ++i)
    {
        const auto& [count, type, size, normalised] = instanceElements[i];
        glEnableVertexAttribArray(i + vertexSize);
        glVertexAttribPointer(i + vertexSize, count, type, normalised, static_cast<int>(vertexBufferLayout.GetInstanceStride()), reinterpret_cast<const void*>(offset));
        glVertexAttribDivisor(i + vertexSize, 1);
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
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

void VertexArray::AddBuffer(const VertexBuffer& vertexBuffer, const VertexBufferLayout& vertexBufferLayout) const
{
    Bind();

    vertexBuffer.Bind();

    const vector<VertexBufferElement>& elements = vertexBufferLayout.GetElements();

    uint32 offset = 0;
    for (size_t i{ 0 }; i < elements.size(); ++i)
    {
        const VertexBufferElement& e = elements[i];
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(i, e.count, e.type, e.normalised, static_cast<int>(vertexBufferLayout.GetStride()), reinterpret_cast<const void*>(offset));
        offset += e.count * e.size;
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
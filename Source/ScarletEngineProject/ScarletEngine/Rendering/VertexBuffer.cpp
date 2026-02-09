#include "ScarletEnginePch.h"
#include "VertexBuffer.h"

#include <glew/glew.h>

namespace Scarlet
{

VertexBuffer::VertexBuffer(const uint32 size)
{
    glCreateBuffers(1, &mId);

    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);
}

VertexBuffer::VertexBuffer(const float* vertices, const uint32 size)
{
    glCreateBuffers(1, &mId);

    Bind();
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_DYNAMIC_DRAW);
}

VertexBuffer::~VertexBuffer()
{
    glDeleteBuffers(1, &mId);
}

void VertexBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void VertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VertexBuffer::SetData(const float* vertices, const uint32 size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, mId);

    glBufferSubData(GL_ARRAY_BUFFER, 0, size, vertices);
}

} // Namespace Scarlet.

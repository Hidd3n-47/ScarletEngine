#include "ScarletEnginePch.h"
#include "IndexBuffer.h"

#include <glew/glew.h>

namespace Scarlet
{

IndexBuffer::IndexBuffer(const size_t count)
{
    glCreateBuffers(1, &mId);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long long>(count * sizeof(uint32)), nullptr, GL_DYNAMIC_DRAW);
}

IndexBuffer::IndexBuffer(const uint32* indices, const size_t count)
{
    glCreateBuffers(1, &mId);
    Bind();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, static_cast<long long>(count * sizeof(uint32)), indices, GL_DYNAMIC_DRAW);
}

IndexBuffer::~IndexBuffer()
{
    glDeleteBuffers(1, &mId);
}

void IndexBuffer::Bind() const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);
}

void IndexBuffer::Unbind()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IndexBuffer::SetData(const uint32* indices, const uint32 count) const
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mId);

    glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, static_cast<long long>(count * sizeof(uint32)), indices);
}

} // Namespace Scarlet.
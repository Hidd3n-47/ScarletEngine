#include "ScarletEnginePch.h"
#include "InstanceBuffer.h"

#include <glew/glew.h>

#include <ScarletMath/Math.h>

namespace Scarlet
{

InstanceBuffer::InstanceBuffer(const size_t maxSize)
{
    glCreateBuffers(1, &mId);

    Bind();
    glBufferData(GL_ARRAY_BUFFER, static_cast<long long>(maxSize), nullptr, GL_DYNAMIC_DRAW);

}

InstanceBuffer::~InstanceBuffer()
{
    glDeleteBuffers(1, &mId);
}

void InstanceBuffer::Bind() const
{
    glBindBuffer(GL_ARRAY_BUFFER, mId);
}

void InstanceBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void InstanceBuffer::SetData(const void* instances, const size_t size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, mId);

    glBufferSubData(GL_ARRAY_BUFFER, 0, static_cast<long long>(size), instances);
}

} // Namespace Scarlet.

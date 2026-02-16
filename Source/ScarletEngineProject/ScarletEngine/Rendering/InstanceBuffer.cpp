#include "ScarletEnginePch.h"
#include "InstanceBuffer.h"

#include <glew/glew.h>

#include <ScarletMath/Math.h>

namespace Scarlet
{

InstanceBuffer::InstanceBuffer(const uint32 maxSize, const uint32 vertexBufferLayoutSize)
{
    glCreateBuffers(1, &mId);

    Bind();
    glBufferData(GL_ARRAY_BUFFER, maxSize, nullptr, GL_DYNAMIC_DRAW);

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

void InstanceBuffer::SetData(const void* instances, const uint32 size) const
{
    glBindBuffer(GL_ARRAY_BUFFER, mId);

    glBufferSubData(GL_ARRAY_BUFFER, 0, size, instances);
}

} // Namespace Scarlet.

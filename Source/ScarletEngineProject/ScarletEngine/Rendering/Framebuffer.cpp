#include "ScarletEnginePch.h"
#include "Framebuffer.h"

#include <glew/glew.h>

namespace Scarlet
{

Framebuffer::Framebuffer(const uint32 width, const uint32 height)
{
    Create(width, height);

    Unbind();
}

Framebuffer::~Framebuffer()
{
    Free();
}

void Framebuffer::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, mId);
    glBindTexture(GL_TEXTURE_2D, mColorAttachmentId);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilAttachmentId);
}

void Framebuffer::Unbind()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindRenderbuffer(GL_RENDERBUFFER, 0);
}

void Framebuffer::Resize(const uint32 width, const uint32 height)
{
    Free();

    Create(width, height);
}

void Framebuffer::Create(const uint32 width, const uint32 height)
{
    glGenFramebuffers(1, &mId);
    glBindFramebuffer(GL_FRAMEBUFFER, mId);

    glGenTextures(1, &mColorAttachmentId);
    glBindTexture(GL_TEXTURE_2D, mColorAttachmentId);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, static_cast<int>(width), static_cast<int>(height), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, mColorAttachmentId, 0);

    glGenRenderbuffers(1, &mDepthStencilAttachmentId);
    glBindRenderbuffer(GL_RENDERBUFFER, mDepthStencilAttachmentId);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, static_cast<int>(width), static_cast<int>(height));
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, mDepthStencilAttachmentId);

    SCARLET_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE && "Framebuffer is incomplete!");
}

void Framebuffer::Free()
{
    glDeleteFramebuffers(1, &mId);
    glDeleteTextures(1, &mColorAttachmentId);
    glDeleteRenderbuffers(1, &mDepthStencilAttachmentId);

    mId = 0;
    mColorAttachmentId        = 0;
    mDepthStencilAttachmentId = 0;
}

} // Namespace Scarlet.
#include "ScarletEnginePch.h"
#include "Texture.h"

#include <glew/glew.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

namespace Scarlet::Resource
{

Texture::Texture(const char* filepath)
{
    int width, height, channels = 0;

    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = stbi_load(filepath, &width, &height, &channels, 0);

    SCARLET_ASSERT(data && "Failed to load image at the given path: {}", filepath);
    SCARLET_ASSERT(channels == 4 && "Incorrect number of channels for the image at the given path: {}", filepath);

    mWidth  = width;
    mHeight = height;

    glCreateTextures(GL_TEXTURE_2D, 1, &mId);
    glTextureStorage2D(mId, 1, GL_RGBA8, width, height);

    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glTextureSubImage2D(mId, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);
}

Texture::~Texture()
{
    glDeleteTextures(1, &mId);
}

void Texture::Bind(const uint32 slot) const
{
    glBindTextureUnit(slot, mId);
}

} // Namespace Scarlet::Resource.
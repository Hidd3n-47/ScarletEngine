#include "ScarletEnginePch.h"
#include "CubeMapTexture.h"

#include <glew/glew.h>

#include <stb_image/stb_image.h>

namespace Scarlet::Resource
{

CubeMapTexture::CubeMapTexture(const char* filepath)
{
    const std::string filepaths[] =
    {
        std::string{ filepath } + "px.png",
        std::string{ filepath } + "nx.png",

        std::string{ filepath } + "pz.png",
        std::string{ filepath } + "nz.png",

        std::string{ filepath } + "py.png",
        std::string{ filepath } + "ny.png",

    };

    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &mId);

    glTextureParameteri(mId, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(mId, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(mId, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    int width, height, channels = 0;

    stbi_set_flip_vertically_on_load(1);

    stbi_uc* data = stbi_load(filepaths[0].c_str(), &width, &height, &channels, 0);

    SCARLET_ASSERT(data && "Failed to load image at the given path: {}", filepaths[0].c_str());
    SCARLET_ASSERT(channels == 4 && "Incorrect number of channels for the image at the given path: {}", filepaths[0].c_str());

    glTextureStorage2D(mId, 1, GL_RGBA8, width, height);
    glTextureSubImage3D(mId, 0, 0, 0, 0, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

    stbi_image_free(data);

    mWidth  = width;
    mHeight = height;

    for (int i{ 1 }; i < 6; ++i)
    {
        data = stbi_load(filepaths[i].c_str(), &width, &height, &channels, 0);

        SCARLET_ASSERT(data && "Failed to load image at the given path: {}", filepaths[i].c_str());
        SCARLET_ASSERT(channels == 4 && "Incorrect number of channels for the image at the given path: {}", filepaths[i].c_str());

        glTextureSubImage3D(mId, 0, 0, 0, i, width, height, 1, GL_RGBA, GL_UNSIGNED_BYTE, data);

        mWidth = width;
        mHeight = height;

        stbi_image_free(data);
    }
}

CubeMapTexture::~CubeMapTexture()
{
    glDeleteTextures(1, &mId);
}

void CubeMapTexture::Bind(const uint32 slot) const
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, mId);
}

} // Namespace Scarlet::Resource.
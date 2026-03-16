#pragma once

namespace Scarlet::Resource
{
/**
 * @class CubeMapTexture: A class representing a 3D image that can be loaded from disk. This class handles uploading of buffer data to the GPU.
 */
class CubeMapTexture
{
public:
    CubeMapTexture(const Filepath& filepath);
    ~CubeMapTexture();

    /**
     * @brief Bind a texture to the graphics pipeline.
     * @param slot The slot the texture is loaded into.
     */
    void Bind(const uint32 slot = 0) const;
private:
    uint32 mId;
    uint32 mWidth, mHeight;
};

} // Namespace Scarlet::Resource.
#pragma once

#include <ScarletMath/Math.h>

namespace Scarlet::Resource
{

class ILazyLoadAsset;

/**
 * @class Material: A structure to hold the information relating to a material. \\n
 * A material contains information about how a mesh looks and reacts to the environment.
 */
class Material
{
public:
    Material(const Filepath& filepath);

    WeakHandle<ILazyLoadAsset> texture;
    Math::Vec3 ambientColor{ 1.0f };
    Math::Vec3 diffuseColor{ 1.0f };

    [[nodiscard]] inline bool operator==(const Material& other) const { return texture == other.texture && ambientColor == other.ambientColor; }
};

} // Namespace Scarlet::Resource.
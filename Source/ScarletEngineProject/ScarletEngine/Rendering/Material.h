#pragma once

namespace Scarlet::Resource
{

class Texture;

/**
 * @struct Material: A structure to hold the information relating to a material. \\n
 * A material contains information about how a mesh looks and reacts to the environment.
 */
struct Material
{
    WeakHandle<Texture> texture;

    [[nodiscard]] inline bool operator==(const Material& other) const { return texture == other.texture; }
};

} // Namespace Scarlet::Resource.
#pragma once

#include "Material.h"

namespace Scarlet
{

namespace Resource
{
class Mesh;
} // Namespace Resource.

/**
 * @struct RenderGroup: A structure to hold a material and mesh. This group is used for render commands to \\n
 * split render commands based off different materials and meshes. A custom hash is used to ensure that either \\n
 * a different material and/or mesh will result in a new \ref RenderGroup. \\n
 * This allows for render commands to be instanced together for similar meshes or materials.
 */
struct RenderGroup
{
    Resource::Material material;
    WeakHandle<Resource::Mesh> mesh;

    [[nodiscard]] inline bool operator==(const RenderGroup& other) const
    {
        return material == other.material && mesh == other.mesh;
    }
};

} // Namespace Scarlet.

namespace std
{

template <>
struct hash<Scarlet::RenderGroup>
{
    size_t operator()(const Scarlet::RenderGroup& renderGroup) const noexcept
    {
        size_t hashValue = 0;

        SCARLET_ASSERT(renderGroup.material.texture.IsValid() && "Invalid texture trying to be hashed.");
        SCARLET_ASSERT(renderGroup.mesh.IsValid() && "Invalid mesh trying to be hashed.");

        // 0x9e3779b9 a prime number used in hashing.
        // The hashing algorithm below is a well known hash combination function.
        hashValue ^= std::hash<void*>{}(renderGroup.material.texture.GetRawPtr()) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);
        hashValue ^= std::hash<void*>{}(renderGroup.mesh.GetRawPtr()) + 0x9e3779b9 + (hashValue << 6) + (hashValue >> 2);

        return hashValue;
    }
};

} // Namespace std.

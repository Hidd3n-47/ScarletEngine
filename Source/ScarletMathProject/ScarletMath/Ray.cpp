#include "Ray.h"

#include <algorithm>

#include "Trig.h"

namespace Scarlet::Math
{

float Ray::RayIntersectsWithBoundingBox(const Ray& ray, const BoundingBox& box)
{
    const Vec3 rotation       = box.GetRotation();
    const Mat3 rotationMatrix = Trig::RotationMatrixRadians(rotation.z, rotation.x, rotation.y);

    const Mat3 invRotation = Transpose(rotationMatrix);

    const Vec3 localOrigin = invRotation * (ray.mOrigin - box.GetTranslation());
    const Vec3 localDir    = Normalize(invRotation * ray.mDirection);

    const Vec3 minBound    = box.GetLocalMinimumBound();
    const Vec3 maxBound    = box.GetLocalMaximumBound();

    float tMin = 0.0f;
    float tMax = std::numeric_limits<float>::max();

    // For loop to go through each component of the vector: x, y and z.
    for (int i = 0; i < 3; ++i)
    {
        if (IsEqual(localDir[i], 0.0))
        {
            // Parallel lines.
            if (localOrigin[i] < minBound[i] || localOrigin[i] > maxBound[i])
            {
                return -1.0f;
            }

            // Zero direction in this axis, so continue to prevent dividing by zero.
            continue;
        }

        const float ood = 1.0f / localDir[i];

        float t1 = (minBound[i] - localOrigin[i]) * ood;
        float t2 = (maxBound[i] - localOrigin[i]) * ood;

        if (t1 > t2)
        {
            std::swap(t1, t2);
        }

        tMin = std::max(tMin, t1);
        tMax = std::min(tMax, t2);

        if (tMin > tMax)
        {
            return -1.0f;
        }
    }

    return tMin > 0.0f ? tMin : tMax;
}

} // Namespace Scarlet::Math.
#pragma once

#include "BoundingBox.h"

namespace Scarlet::Math
{
/**
 * @class Ray: A class to represent a ray. A ray is a line that starts at an origin and goes infinitely in a direction
 * @remark Ray's are unidirectional meaning they do not go in opposite direction, strictly from origin to some positive scalar multiple t of t*direction.
 */
class Ray
{
public:
    Ray() = default;
    explicit inline Ray(const Vec3 origin, const Vec3 direction)
        : mOrigin(origin), mDirection(direction)
    { /* Empty.*/ }

    /**
     * @brief Check if a ray intersects with a bounding box.
     * @see BoundingBox
     * @param ray The ray that potentially is intersecting the bounding box.
     * @param box The box that the ray is potentially intersecting with.
     * @return \c True if the ray intersects with the bounding box, \c false otherwise.
     */
    static inline bool RayIntersectsWithBoundingBox(const Ray& ray, const BoundingBox& box)
    {
        const Vec3 rayDir    = Quat::RotatePoint(ray.GetDirection(), Quat { box.GetRotation().Inverse() });
        const Vec3 rayOrigin = Quat::RotatePoint(ray.GetOrigin() - box.GetTranslation(), Quat { box.GetRotation().Inverse() });
        const Vec3 boxOrigin = box.GetLocalCenter();

        const float tMin = ( rayDir.x * (boxOrigin.x - rayOrigin.x) + rayDir.y * (boxOrigin.y - rayOrigin.y) + rayDir.z * (boxOrigin.z - rayOrigin.z) )
                            / Dot(rayDir, rayDir);

        const Vec3 closestPoint = rayOrigin + tMin * rayDir;

        const Vec3 rotatedMin = box.GetLocalMinimumBound();
        const Vec3 rotatedMax = box.GetLocalMaximumBound();

        if ((rotatedMin.x <= closestPoint.x && closestPoint.x <= rotatedMax.x) &&
            (rotatedMin.y <= closestPoint.y && closestPoint.y <= rotatedMax.y) &&
            (rotatedMin.z <= closestPoint.z && closestPoint.z <= rotatedMax.z) &&
            (tMin >= 0.0f))
        {
            return true;
        }

        return false;
    }

    /**
     * @brief Get the origin of the ray.
     * @return The origin of the ray.
     */
    [[nodiscard]] inline Vec3 GetOrigin()       const { return mOrigin; }
    /**
     * @brief Get the direction the ray is travelling in.
     * @return The direction the ray is travelling in.
     */
    [[nodiscard]] inline Vec3 GetDirection()    const { return mDirection; }
private:
    Vec3 mOrigin    = { 0.0f, 0.0f, 0.0f};
    Vec3 mDirection = { 1.0f, 0.0f, 0.0f};
};

} // Namespace Scarlet::Math.

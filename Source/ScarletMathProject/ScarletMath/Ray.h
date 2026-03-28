#pragma once

#include "BoundingBox.h"

namespace Scarlet::Math
{
/**
 * @class Ray: A class to represent a ray. A ray is a line that starts at an origin and goes infinitely in a direction
 * @remark Ray's are unidirectional meaning they do not go in opposite direction, strictly from origin to some positive scalar multiple \c t of \c t*direction.
 */
class Ray
{
public:
    Ray() = default;
    explicit inline Ray(const Vec3 origin, const Vec3 direction)
        : mOrigin(origin), mDirection(direction)
    { /* Empty.*/ }

    /**
     * @brief Get the origin of the ray.
     * @return The origin of the ray.
     */
    [[nodiscard]] inline Vec3 GetOrigin() const { return mOrigin; }
    /**
     * @brief Get the direction the ray is travelling in.
     * @return The direction the ray is travelling in.
     */
    [[nodiscard]] inline Vec3 GetDirection() const { return mDirection; }

    /**
     * @brief Check if a ray intersects with a \ref BoundingBox.
     * @see BoundingBox
     * @param box The box that the ray is potentially intersecting with.
     * @return The value of \c t required to intersect with the box (@code P = t * RayDir @endcode ). Returns a negative \c t value if no intersection occured.
     */
    float IntersectsWithBoundingBox(const BoundingBox& box) const { return RayIntersectsWithBoundingBox(*this, box); }

    /**
     * @brief Check if a ray intersects with a \ref BoundingBox.
     * @see BoundingBox
     * @param ray The ray that potentially is intersecting the bounding box.
     * @param box The box that the ray is potentially intersecting with.
     * @return The value of \c t required to intersect with the box (@code P = t * RayDir @endcode ). Returns a negative \c t value if no intersection occured.
     */
    static float RayIntersectsWithBoundingBox(const Ray& ray, const BoundingBox& box);
private:
    Vec3 mOrigin    = { 0.0f, 0.0f, 0.0f};
    Vec3 mDirection = { 1.0f, 0.0f, 0.0f};
};

} // Namespace Scarlet::Math.

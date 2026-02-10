#pragma once

#include "Quat.h"

namespace Scarlet::Math
{
/**
 * @class BoundingBox: A class to represent a box in space that has a local minimum and maximum bound.
 */
class BoundingBox
{
public:
    BoundingBox() = default;
    /**
     * @brief Construct a bounding box with given (local) minimum and maximum bounds.
     * @remark The bounds are local, i.e. should not contain any translation or rotation.
     * @param minimumBound 
     * @param maximumBound
     */
    BoundingBox(const Vec3 minimumBound, const Vec3 maximumBound)
        : mMinimumBound{ minimumBound }, mMaximumBound{ maximumBound }
    { /* Empty. */ }

    /**
     * @brief Get the local space center/average of the bounding box's bounds.
     * @return The (local space) average of the bounding box's bounds.
     */
    [[nodiscard]] inline Vec3 GetLocalCenter() const { return (mMinimumBound + mMaximumBound) * 0.5f; }

    /**
     * @brief Get the local space minimum bound of the bounding box.
     * @return The (local space) minimum bound.
     */
    [[nodiscard]] inline Vec3 GetLocalMinimumBound() const { return mMinimumBound; }
    /**
     * @brief Get the local space maximum bound of the bounding box.
     * @return The (local space) maximum bound.
     */
    [[nodiscard]] inline Vec3 GetLocalMaximumBound() const { return mMaximumBound; }

    /**
     * @brief Get the world translation of the bounding box.
     * @remark The world translation will have to be externally set before it can be correctly retrieved.
     * @return The translation of the bounding box.
     */
    [[nodiscard]] inline Vec3 GetTranslation()     const { return mTranslation; }
    /**
     * @brief Get the world rotation of the bounding box.
     * @remark The world rotation will have to be externally set before it can be correctly retrieved.
     * @return The rotation of the bounding box.
     */
    [[nodiscard]] inline const Quat& GetRotation() const { return mRotation; }

    /**
     * @brief Set the world translation of the bounding box.
     * @param translation The value the translation is being set to.
     */
    void SetTranslation(const Vec3 translation)     { mTranslation  = translation; }
    /**
     * @brief Set the world rotation of the bounding box.
     * @param rotation The value the rotation is being set to.
     */
    void SetRotation(const Quat& rotation)          { mRotation     = rotation; }
    /**
     * @brief Set the local minimum bound of the bounding box.
     * @param minimumBound The value that the minimum bound is being set to. Ensure that this value is local space and not world space.
     */
    void SetMinimumBound(const Vec3 minimumBound)   { mMinimumBound = minimumBound; }
    /**
     * @brief Set the local maximum bound of the bounding box.
     * @param maximumBound The value that the maximum bound is being set to. Ensure this value is local space and not world space.
     */
    void SetMaximumBound(const Vec3 maximumBound)   { mMaximumBound = maximumBound; }
private:
    Vec3 mTranslation  = { 0.0f, 0.0f, 0.0f };
    Quat mRotation     = Quat::Identity();

    Vec3 mMinimumBound = { -0.1f, -0.5f, -0.5f };
    Vec3 mMaximumBound = {  0.1f,  0.5f,  0.5f };
};

} // Namespace Scarlet::Math.

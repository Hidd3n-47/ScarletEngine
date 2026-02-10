#pragma once

#include "Math.h"

namespace Scarlet::Math
{

/**
 * @class Quat: A class to represent quaternions which are used for rotations.
 */
class Quat
{
public:
    Quat() = default;
    /**
     * @brief: Construct a quaternion to represent a point in space.<br/>
     * This is useful when trying to rotate a point as quaternion multiplication can be used.
     * @param point The point in space that is being represented in quaternion form.
     */
    explicit Quat(const Vec3& point);
    /**
     * @brief: Construct a quaternion to represent a rotation about a given axis.
     * @param angleRadians The angle of rotation about the axis.
     * @param axis The axis the rotation is about.
     */
    explicit Quat(const double angleRadians, const Vec3& axis);
    /**
     * @brief Construct a quaternion to represent a rotation from the given Yaw, Pitch and Roll rotation.
     * @param yawRadians The angle of rotation (in radians) about the \c z-axis.
     * @param pitchRadians The angle of rotation (in radians) about the \c y-axis.
     * @param rollRadians The angle of rotation (in radians) about the \c x-axis.
     */
    explicit Quat(const double yawRadians, const double pitchRadians, const double rollRadians);
    /**
     * @brief Construct a quaternion from given values of a quaternion.
     * @param w The \c w component of the quaternion, also known as the real value.
     * @param x The \c x component of the quaternion, also known as the \c i coefficients in the imaginary numbers.
     * @param y The \c y component of the quaternion, also known as the \c j coefficients in the imaginary numbers.
     * @param z The \c z component of the quaternion, also known as the \c k coefficients in the imaginary numbers.
     */
    explicit Quat(const double w, const double x, const double y, const double z);
    /**
     * @brief Construct a quaternion from given values of a quaternion.
     * @param vec The \c w component of the quaternion, also known as the real value.
     */
    explicit Quat(const Vec4& vec);

    /**
     * @brief Normalise the quaternion to have a length of 1.
     */
    void Normalize();

    /**
     * @brief Rotate a given point by a given rotation.
     * @param point The point being rotated
     * @param quaternion The quaternion used to rotate the point.
     * @return The rotated point.
     */
    static Vec3 RotatePoint(const Vec3& point, const Quat& quaternion);

    /**
     * @brief Get if the quaternion is an identity quaternion.
     * @return \c True if identity quaternion, \c false otherwise.
     */
    [[nodiscard]] inline bool IsIdentity() const { return IsEqual(mX, mY) && IsEqual(mY, mZ) && IsEqual(mZ, 0.0f) && IsEqual(mW, 1.0f); }
    /**
     * @breif Get the axis of rotation. This is also the coefficients of the imaginary parts.
     * @return A vector representing the coefficients of the imaginary parts, also known as the rotation axis.
     */
    [[nodiscard]] inline Vec3 GetAxis() const { return { mX, mY, mZ }; }

    /**
     * @brief Get the inverse of the quaternion.
     * @remark Note this should only be done on unit quaternions as it uses the conjugate for optimisations reasons.
     * @return The inverse of this quaternion.
     */
    [[nodiscard]] inline Quat Inverse() const { return Quat{ mW, -mX, -mY, -mZ }; }
    /**
     * @breif Get the matrix representation of the rotation.
     * @return The matrix which represents the rotation of the quaternion.
     */
    [[nodiscard]] Mat4 GetRotationMatrix() const;

    /**
     * @brief Get the Yaw, Pitch and Roll rotations for the quaternion.
     * @remark Angles are return measured in radians.
     * @param yaw The angle representing the rotation in the 'yaw' axis (i.e. \c z-axis).
     * @param pitch The angle representing the rotation in the 'pitch' axis (i.e. \c y-axis).
     * @param roll The angle representing the rotation in the 'roll' axis (i.e. \c x-axis).
     */
    //inline void GetYawPitchRoll(double& yaw, double& pitch, double& roll) const { yaw = mYaw; pitch = mPitch; roll = mRoll; }
    void GetYawPitchRoll(double& yaw, double& pitch, double& roll) const;

    /**
     * @brief Set the Yaw, Pitch and Roll rotations for the quaternion.
     * @remark Angles are measured in radians.
     * @param yaw The angle representing the rotation in the 'yaw' axis (i.e. \c z-axis).
     * @param pitch The angle representing the rotation in the 'pitch' axis (i.e. \c y-axis).
     * @param roll The angle representing the rotation in the 'roll' axis (i.e. \c x-axis).
     */
    inline void SetYawPitchRoll(const double yaw, const double pitch, const double roll) { *this = Quat { yaw, pitch, roll }; }

    inline void operator*=(const Quat& rhs)
    {
        *this = Multiply(*this, rhs);
    }

    inline Quat operator*(const Quat& rhs) const
    {
        return Multiply(*this, rhs);
    }

    inline bool operator==(const Quat& rhs) const
    {
        return IsEqual(mX, rhs.mX) &&
               IsEqual(mY, rhs.mY) &&
               IsEqual(mZ, rhs.mZ) &&
               IsEqual(mW, rhs.mW);
    }

    inline bool operator==(const Quat& rhs)
    {
        return IsEqual(mX, rhs.mX) &&
               IsEqual(mY, rhs.mY) &&
               IsEqual(mZ, rhs.mZ) &&
               IsEqual(mW, rhs.mW);
    }

    /**
     * @brief Get the rotation needed to rotate a vector, \c vectorA, to another vector, \c vectorB.
     * @param vectorA The normalised original vector direction that rotated to a certain vector direction. Ensure that the vector is normalised before use.
     * @param vectorB The normalised vector direction that the original vector is being rotated to. Ensure that the vector is normalised before use.
     * @return The rotation needed to rotate the original vector to the direction of the other.
     */
    static Quat GetRotationToRotateVectorToVector(const Vec3& vectorA, const Vec3& vectorB);

    /**
     * Get a vector 4 representation of the quaternion.
     * @return A vector 4 representation of the quaternion as (\c x, \c y, \c z, \c w), and not as (\c w, \c *v*), this prevents confusion like vec4.x giving the \c w component.
     */
    [[nodiscard]] inline Vec4 ToVector4() const { return { mX, mY, mZ, mW }; }

    /**
     * @breif Get an identity quaternion.
     * @return An identity quaternion.
     */
    [[nodiscard]] static Quat Identity() { return Quat{ 1.0f, 0.0f, 0.0f, 0.0f }; }
private:
    double mW = 0.0f, mX = 0.0f, mY = 0.0f, mZ = 0.0f;

    static Quat Multiply(const Quat& lhs, const Quat& rhs);
};

} // Namespace Scarlet::Math.

#include "Quat.h"

#include <numbers>

#include "Trig.h"

namespace Scarlet::Math
{

Quat::Quat(const Vec3& point)
    : mX (point.x)
    , mY (point.y)
    , mZ (point.z)
{
    // Empty.
}

Quat::Quat(const double angleRadians, const Vec3& axis)
{
    const double angleCorrected = Trig::CorrectAngleTo0To2PiRange(angleRadians);
    const double halfAngle = angleCorrected * 0.5;

    mX = axis.x * Trig::Sin(halfAngle);
    mY = axis.y * Trig::Sin(halfAngle);
    mZ = axis.z * Trig::Sin(halfAngle);

    mW = Trig::Cos(halfAngle);
}

Quat::Quat(const double yawRadians, const double pitchRadians, const double rollRadians)
{
    const Quat x { Trig::CorrectAngleTo0To2PiRange(rollRadians)  , { 1.0f, 0.0f, 0.0f } };
    const Quat y { Trig::CorrectAngleTo0To2PiRange(pitchRadians) , { 0.0f, 1.0f, 0.0f } };
    const Quat z { Trig::CorrectAngleTo0To2PiRange(yawRadians)   , { 0.0f, 0.0f, 1.0f } };

    const Quat rotation = z * y * x;
    mW = rotation.mW;
    mX = rotation.mX;
    mY = rotation.mY;
    mZ = rotation.mZ;
}

Quat::Quat(const double w, const double x, const double y, const double z)
    : mW(w), mX(x), mY(y), mZ(z)
{
    // Empty.
}

Quat::Quat(const Vec4& vec)
    : mW(vec.w), mX(vec.x), mY(vec.y), mZ(vec.z)
{
    // Empty.
}

void Quat::Normalize()
{
    if (const double length = Sqrt(mX * mX + mY * mY + mZ * mZ + mW * mW); !IsEqual(length, 0.0))
    {
        mX /= length;
        mY /= length;
        mZ /= length;
        mW /= length;
    }
}

Vec3 Quat::RotatePoint(const Vec3& point, const Quat& quaternion)
{
    const Quat pointQuat { point };
    return (quaternion * pointQuat * quaternion.Inverse()).GetAxis();
}

Mat4 Quat::GetRotationMatrix() const
{
    // Todo/Remark, leaving this here for when we do SIMD. This could be an interesting read to do performance on below, then on the first optimised by factoring, and then SIMD.
    /*return {
    1.f - 2.f * mY * mY - 2.f * mZ * mZ  ,        2.f * mX * mY - 2.f * mW * mZ  ,        2.f * mX * mZ + 2.f * mW * mY  ,  0.f,
    2.f * mX * mY + 2.f * mW * mZ        ,  1.f - 2.f * mX * mX - 2.f * mZ * mZ  ,        2.f * mY * mZ - 2.f * mW * mX  ,  0.f,
    2.f * mX * mZ - 2.f * mW * mY        ,        2.f * mY * mZ + 2.f * mW * mX  ,  1.f - 2.f * mX * mX - 2.f * mY * mY  ,  0.f,
    0.f                                  ,                                  0.f  ,                                  0.f  ,  1.f
    };*/

    // Todo prime example of SIMD.
    const double xx = mX * mX;
    const double yy = mY * mY;
    const double zz = mZ * mZ;

    const double xy = mX * mY;
    const double xz = mX * mZ;

    const double yz = mY * mZ;

    const double wx = mW * mX;
    const double wy = mW * mY;
    const double wz = mW * mZ;

    return {
        1. - 2. * (yy + zz) ,       2. * (xy - wz) ,      2. * (xz + wy) , 0.,
             2. * (xy + wz) , 1.  - 2. * (xx + zz) ,      2. * (yz - wx) , 0.,
             2. * (xz - wy) ,       2. * (yz + wx) , 1. - 2. * (xx + yy) , 0.,
                         0. ,                   0. ,                  0. , 1.
    };
}

void Quat::GetYawPitchRoll(double& yaw, double& pitch, double& roll) const
{
    const double xx = mX * mX;
    const double yy = mY * mY;
    const double zz = mZ * mZ;

    const double xy = mX * mY;
    const double xz = mX * mZ;

    const double yz = mY * mZ;

    const double wx = mW * mX;
    const double wy = mW * mY;
    const double wz = mW * mZ;

    const double m11        = 1. - 2. * (yy + zz);
    const double m12        = 2. * (xy + wz);
    const double negM13     = 2. * (wy - xz);
    const double m23        = 2. * (yz + wx);
    const double m33        = 1. - 2. * (xx + yy);

    const double sinPitch = negM13;

    yaw     = Trig::Atan2(m12, m11);
    pitch   = Trig::Asin(sinPitch);
    roll    = Trig::Atan2(m23, m33);

    if (IsEqual(abs(sinPitch), 1.0))
    {
        pitch = std::numbers::pi / 2. * Sign(sinPitch);

        yaw     = atan2(m12, m11);
        roll    = 0;
    }
}

Quat Quat::GetRotationToRotateVectorToVector(const Vec3& vectorA, const Vec3& vectorB)
{
    // Todo could do with adding some tests for this.
    const float dot = Dot(vectorA, vectorB);

    if (IsEqual(dot, 1.0f))
    {
        return Identity();
    }

    // Todo when writing tests see if this case is needed or if the optimised method will cover this case.
    if (IsEqual(dot, -1.0f))
    {
        // Try to find an orthogonal vector to VectorA.
        Vec3 orthogonal = Cross({ 1.0f, 0.0f, 0.0f }, vectorA);

        // If the magnitude of the orthogonal vector is close to zero, try a different basis vector to check for orthogonality.
        // This would occur if vectorA = (1,0,0), in which case (0,1,0) will guarantee give non-zero cross product.
        if (IsEqual(Magnitude(orthogonal), 0.0f))
        {
            orthogonal = Cross({ 0.0f, 1.0f, 0.0f }, vectorA);
        }

        orthogonal = Scarlet::Math::Normalize(orthogonal);

        return Quat(std::numbers::pi, orthogonal);
    }

    const Vec3 axis = Cross(vectorA, vectorB);

    const double angle = std::sqrt((1 + dot) * 2);
    const double inverseAngle = 1.0 / angle;

    return Quat{ -angle * 0.5, axis.x * inverseAngle, axis.y * inverseAngle, axis.z * inverseAngle };
}

Quat Quat::Multiply(const Quat& lhs, const Quat& rhs)
{
    Quat solutionQuat;

    solutionQuat.mX = lhs.mW * rhs.mX + lhs.mX * rhs.mW + lhs.mY * rhs.mZ - lhs.mZ * rhs.mY;
    solutionQuat.mY = lhs.mW * rhs.mY + lhs.mY * rhs.mW + lhs.mZ * rhs.mX - lhs.mX * rhs.mZ;
    solutionQuat.mZ = lhs.mW * rhs.mZ + lhs.mZ * rhs.mW + lhs.mX * rhs.mY - lhs.mY * rhs.mX;
    solutionQuat.mW = lhs.mW * rhs.mW - lhs.mX * rhs.mX - lhs.mY * rhs.mY - lhs.mZ * rhs.mZ;

    return solutionQuat;
}

} // Namespace Scarlet::Math.

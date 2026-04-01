#pragma once

#include <cmath>
#include <numbers>

#include "Math.h"

namespace Scarlet::Math
{

class Trig
{
public:
    [[nodiscard]] inline static double Sin(const double angleRadians)
    {
        return std::sin(angleRadians);
    }

    [[nodiscard]] inline static double Cos(const double angleRadians)
    {
        return std::cos(angleRadians);
    }

    [[nodiscard]] inline static double Asin(const double value)
    {
        return std::asin(Clamp(value, -1.0, 1.0));
    }

    [[nodiscard]] inline static double Acos(const double value)
    {
        return std::acos(Clamp(value, -1.0, 1.0));
    }

    [[nodiscard]] inline static double Atan(const double value)
    {
        return std::atan(Clamp(value, -1.0, 1.0));
    }

    [[nodiscard]] inline static double Atan2(const double y, const double x)
    {
        return std::atan2(y, x);
    }

    [[nodiscard]] inline static double CorrectAngleTo0To2PiRange(double angle)
    {
        return glm::mod(angle, 2 * std::numbers::pi);
    }

    [[nodiscard]] inline static Mat4 RotationMatrix(const Vec3 rotationDegrees)
    {
        return RotationMatrixRadians(Radians(rotationDegrees.z), Radians(rotationDegrees.x), Radians(rotationDegrees.y));
    }

    [[nodiscard]] inline static Mat4 RotationMatrix(const double yawDegrees, const double pitchDegrees, const double rollDegrees)
    {
        return RotationMatrixRadians(Radians(yawDegrees), Radians(pitchDegrees), Radians(rollDegrees));
    }

    [[nodiscard]] inline static Mat4 RotationMatrixRadians(const double yawRadians, const double pitchRadians, const double rollRadians)
    {
        const double sY = Sin(yawRadians);
        const double cY = Cos(yawRadians);
        const double sP = Sin(pitchRadians);
        const double cP = Cos(pitchRadians);
        const double sR = Sin(rollRadians);
        const double cR = Cos(rollRadians);

        const Mat4 rotateX = {
            1.0, 0.0, 0.0, 0.0,
            0.0,  cP, -sP, 0.0,
            0.0,  sP,  cP, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

        const Mat4 rotateY = {
              cR, 0.0,   sR, 0.0,
             0.0, 1.0,  0.0, 0.0,
             -sR, 0.0,   cR, 0.0,
             0.0, 0.0,  0.0, 1.0
        };

        const Mat4 rotateZ = {
              cY,  -sY, 0.0, 0.0,
              sY,   cY, 0.0, 0.0,
             0.0,  0.0, 1.0, 0.0,
             0.0,  0.0, 0.0, 1.0
        };

        return rotateZ * rotateX * rotateY;
    }

    inline static void DecomposeTransformMatrixDegrees(const Mat4& transformMatrix, Vec3& translation, Vec3& rotation, Vec3& scale)
    {
        translation = Vec3{ transformMatrix[3] };

        Vec3 xVector{ transformMatrix[0] };
        Vec3 yVector{ transformMatrix[1] };
        Vec3 zVector{ transformMatrix[2] };

        scale.x = Magnitude(xVector);
        scale.y = Magnitude(yVector);
        scale.z = Magnitude(zVector);

        xVector /= scale.x;
        yVector /= scale.y;
        zVector /= scale.z;

        // Yaw.
        rotation.z = Degrees(static_cast<float>(CorrectAngleTo0To2PiRange(Atan2(yVector.x, yVector.y))));
        // Pitch.
        rotation.x = Degrees(static_cast<float>(CorrectAngleTo0To2PiRange(Atan2(zVector.y, zVector.z))));
        // Roll.
        rotation.y = Degrees(static_cast<float>(CorrectAngleTo0To2PiRange(Atan2(xVector.z, zVector.z))));
    }
};

} // Namespace Scarlet::Math.

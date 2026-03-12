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
};

} // Namespace Scarlet::Math.

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
};

} // Namespace Scarlet::Math.

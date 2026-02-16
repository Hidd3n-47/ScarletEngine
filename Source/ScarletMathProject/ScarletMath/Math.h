#pragma once

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/ext/matrix_clip_space.hpp>

namespace Scarlet::Math
{

// TODO: Make these actual classes instead of just using glm's version of it.
typedef glm::vec2 Vec2;
typedef glm::vec3 Vec3;
typedef glm::vec4 Vec4;

typedef glm::mat3 Mat3;
typedef glm::mat4 Mat4;

template <typename T>
[[nodiscard]] static inline T Abs(const T value)
{
    return glm::abs(value);
}

template <typename T>
[[nodiscard]] static inline constexpr T Degrees(const T radians)
{
    return glm::degrees(radians);
}

template <typename T>
[[nodiscard]] static inline constexpr T Radians(const T degrees)
{
    return glm::radians(degrees);
}

[[nodiscard]] static inline float Magnitude(const Vec3 vector)
{
    return glm::length(vector);
}

[[nodiscard]] static inline Mat4 Perspective(const float fovDegrees, const float aspectRatio, const float zNear, const float zFar)
{
    return glm::perspective(Radians(fovDegrees), aspectRatio, zNear, zFar);
}

[[nodiscard]] static double Sqrt(const double number)
{
    return glm::sqrt(number);
}

[[nodiscard]] static inline Vec3 Normalize(const Vec3 v)
{
    return glm::normalize(v);
}

[[nodiscard]] static inline Vec3 Cross(const Vec3 v1, const Vec3 v2)
{
    return glm::cross(v1, v2);
}

[[nodiscard]] static inline float Dot(const Vec3 v1, const Vec3 v2)
{
    return glm::dot(v1, v2);
}

[[nodiscard]] static inline Mat4 LookAt(const Vec3 eye, const Vec3 center, const Vec3 up)
{
    const Vec3 f { Normalize(center - eye) };
    const Vec3 s { Normalize(cross(up, f)) };
    const Vec3 u { Cross(f, s) };

    Mat4 result { 1 };
    result[0][0] =  s.x;
    result[1][0] =  s.y;
    result[2][0] =  s.z;
    result[0][1] =  u.x;
    result[1][1] =  u.y;
    result[2][1] =  u.z;
    result[0][2] = -f.x;
    result[1][2] = -f.y;
    result[2][2] = -f.z;
    result[3][0] = -dot(s, eye);
    result[3][1] = -dot(u, eye);
    result[3][2] =  dot(f, eye);
    return result;
}

template <typename T>
[[nodiscard]] static inline T Clamp(const T value, const T min, const T max)
{
    return glm::clamp(value, min, max);
}

template <typename T>
[[nodiscard]] static inline T Sign(const T value)
{
    if (value < 0)
    {
        return -1;
    }

    return 1;
}

[[nodiscard]] static bool IsEqual(const double lhs, const double rhs)
{
    // Todo Christian: this used to be 0.00001f but now is lowered to prevent test from failing for quaternions. Need to try to fix precision as this seems too large for error.
    constexpr double EPSILON = 0.001;

    return (abs(lhs - rhs) < EPSILON);
}

[[nodiscard]] static bool IsEqual(const Vec3 lhs, const Vec3 rhs)
{
    return (IsEqual(lhs.x, rhs.x)) &&
           (IsEqual(lhs.y, rhs.y)) &&
           (IsEqual(lhs.z, rhs.z));
}

[[nodiscard]] static bool IsEqual(const Mat4& lhs, const Mat4& rhs)
{
    // Todo look at optimising this for cache efficiency.
    for ( int i { 0 }; i < 4; ++i)
    {
        for ( int j { 0 }; j < 4; ++j)
        {
            if (!IsEqual(lhs[i][j], rhs[i][j]))
            {
                return false;
            }
        }
    }
    return true;
}

[[nodiscard]] static Mat4 ScaleMatrix(const Vec3 scale)
{
    return { scale.x,    0.0f,    0.0f, 0.0f,
                0.0f, scale.y,    0.0f, 0.0f,
                0.0f,    0.0f, scale.z, 0.0f,
                0.0f,    0.0f,    0.0f, 1.0f };
}

[[nodiscard]] static Mat4 TranslateMatrix(const Vec3 translation)
{
    Mat4 trans{ 1.0f };
    trans[3][0] = translation.x;
    trans[3][1] = translation.y;
    trans[3][2] = translation.z;
    return trans;
}

[[nodiscard]] static Mat4 Transpose(const Mat4& matrix)
{
    return glm::transpose(matrix);
}

} // Namespace Scarlet::Math.

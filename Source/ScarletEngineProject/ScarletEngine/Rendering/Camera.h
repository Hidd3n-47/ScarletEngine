#pragma once

#include <ScarletMath/Math.h>

namespace Scarlet
{

struct Camera
{
public:
    inline void UpdateViewAndProjectionMatrix(const Math::Vec3& eyePosition)
    {
        mViewMatrix       = Math::LookAt(eyePosition, eyePosition + mForwardVector, mUpVector);
        mProjectionMatrix = Math::Perspective(mFov, mAspectRatio, mNearPlane, mFarPlane);
    }

    inline const Math::Mat4& GetViewMatrix()       const { return mViewMatrix; }
    inline const Math::Mat4& GetProjectionMatrix() const { return mProjectionMatrix; }

    inline Math::Vec3 GetForwardVector() const { return mForwardVector; }
    inline Math::Vec3 GetRightVector()   const { return mRightVector; }
    inline Math::Vec3 GetUpVector()      const { return mUpVector; }

    inline void SetViewMatrix(const Math::Mat4& viewMatrix)       { mViewMatrix = viewMatrix; }
    inline void SetProjectionMatrix(const Math::Mat4& projMatrix) { mProjectionMatrix = projMatrix; }

    inline void SetForwardVector(const Math::Vec3& forward) { mForwardVector = forward; }
    inline void SetRightVector(const Math::Vec3& right)     { mRightVector = right; }
    inline void SetUpVector(const Math::Vec3& up)           { mUpVector = up; }
    inline void SetAspectRatio(const float& aspectRatio)    { mAspectRatio = aspectRatio; }

    [[nodiscard]]inline float GetAspectRatio() const { return mAspectRatio; }
private:
    Math::Mat4 mViewMatrix      { 1.0f };
    Math::Mat4 mProjectionMatrix{ 1.0f };

    Math::Vec3 mForwardVector{ 0.0f, -1.0f, 0.0f };
    Math::Vec3 mRightVector  { 1.0f,  0.0f, 0.0f };
    Math::Vec3 mUpVector     { 0.0f,  0.0f, 1.0f };

    float mAspectRatio = 16.0f / 9.0f;
    float mNearPlane   = 0.1f;
    float mFarPlane    = 1000.0f;
    float mFov         = 60.0f;
};

} // Namespace Scarlet.
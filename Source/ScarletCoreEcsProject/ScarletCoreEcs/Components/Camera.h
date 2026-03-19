#pragma once

#include "ScarletCoreEcs/Defines.h"

#include <ScarletMath/Math.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct COMPONENT_API Camera
{
    inline void UpdateViewAndProjectionMatrix(const Math::Vec3& eyePosition, const Math::Mat4& rotationMatrix)
    {
        const Math::Vec3 forwardVector = rotationMatrix[1];
        const Math::Vec3 upVector      = rotationMatrix[2];
        viewMatrix       = Math::LookAt(eyePosition, eyePosition + forwardVector, upVector);
        projectionMatrix = Math::Perspective(fov, aspectRatio, nearPlane, farPlane);
    }

    Math::Mat4 viewMatrix{ 1.0f };
    Math::Mat4 projectionMatrix{ 1.0f };

    Math::Vec3 forwardVector{ 0.0f, 1.0f, 0.0f };
    Math::Vec3 rightVector  { 1.0f, 0.0f, 0.0f };
    Math::Vec3 upVector     { 0.0f, 0.0f, 1.0f };

    float aspectRatio = 16.0f / 9.0f;
    float nearPlane   = 0.1f;
    float farPlane    = 1000.0f;
    float fov         = 60.0f;

    SCARLET_COMPONENT(Camera)
};

} // Namespace Scarlet::Component
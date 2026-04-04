#pragma once

#include <ScarletMath/Math.h>

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct Camera
{
    Math::Mat4 viewMatrix{ 1.0f };
    Math::Mat4 projectionMatrix{ 1.0f };

    Math::Vec3 forwardVector{ 0.0f, 1.0f, 0.0f };
    Math::Vec3 rightVector  { 1.0f, 0.0f, 0.0f };
    Math::Vec3 upVector     { 0.0f, 0.0f, 1.0f };

    float aspectRatio = 16.0f / 9.0f;
    float nearPlane   = 0.1f;
    float farPlane    = 1000.0f;
    float fov         = 60.0f;

    bool dirty = true;

    SCARLET_COMPONENT(Camera)
};

} // Namespace Scarlet::Component
#pragma once

#include <ScarletTestTemplate/Core/TestRegistry.h>

#include <ScarletMath/Ray.h>

class BoundingBoxIntersectionTests
{
public:
    explicit inline BoundingBoxIntersectionTests(Scarlet::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnXAxis", BoundingBoxIntersectionOnXAxis);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionFails", BoundingBoxIntersectionFails);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnTransformedBox", BoundingBoxIntersectionOnTransformedBox);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnTranslatedAndRotatedBox", BoundingBoxIntersectionOnTranslatedAndRotatedBox);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionOnRotatedBox", BoundingBoxIntersectionOnRotatedBox);
        testRegistry->AddTestCase("Bounding Box Intersection Tests", "BoundingBoxIntersectionFailsWhenRayInFrontOfBox", BoundingBoxIntersectionFailsWhenRayInFrontOfBox);
    }

    inline static bool BoundingBoxIntersectionOnXAxis()
    {
        const Scarlet::Math::Ray ray { { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f} };
        constexpr Scarlet::Math::BoundingBox box;

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box) > 0.0f;
    }

    inline static bool BoundingBoxIntersectionFails()
    {
        const Scarlet::Math::Ray ray { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f} };
        constexpr Scarlet::Math::BoundingBox box;

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box) < 0.0f;
    }

    inline static bool BoundingBoxIntersectionOnTransformedBox()
    {
        const Scarlet::Math::Ray ray { { 1.0f, 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f} };
        Scarlet::Math::BoundingBox box;
        box.SetTranslation({0.0f, 1.0f, 0.0f});

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box) > 0.0f;
    }

    inline static bool BoundingBoxIntersectionOnTranslatedAndRotatedBox()
    {
        const Scarlet::Math::Ray ray { { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        Scarlet::Math::BoundingBox box { { 0.0f, -1.0f, -1.0f}, { 0.5f, 1.0f, 1.0f} };
        box.SetTranslation({1.0f, 0.0f, 0.0f });
        box.SetRotation(Scarlet::Math::Vec3{ Scarlet::Math::Radians(90.f), 0.0f, 0.0f });

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box) > 0.0f;
    }

    inline static bool BoundingBoxIntersectionOnRotatedBox()
    {
        const Scarlet::Math::Ray rayX { { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f} };
        const Scarlet::Math::Ray rayY { { -0.3f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        Scarlet::Math::BoundingBox box;
        box.SetTranslation({0.5f, 0.5f, 0.0f});
        box.SetRotation(Scarlet::Math::Vec3{ Scarlet::Math::Radians(45.f), 0.0f, 0.0f });

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(rayX, box) > 0.0f && Scarlet::Math::Ray::RayIntersectsWithBoundingBox(rayY, box) > 0.0f;
    }

    inline static bool BoundingBoxIntersectionFailsWhenRayInFrontOfBox()
    {
        const Scarlet::Math::Ray ray{ { 1.5f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        constexpr Scarlet::Math::BoundingBox box;

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box) < 0.0f;
    }
};

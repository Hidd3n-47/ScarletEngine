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
    }

    inline static bool BoundingBoxIntersectionOnXAxis()
    {
        const Scarlet::Math::Ray ray { { 1.0f, 0.0f, 0.0f }, { -1.0f, 0.0f, 0.0f} };
        const Scarlet::Math::BoundingBox box;

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionFails()
    {
        const Scarlet::Math::Ray ray { { 1.0f, 0.0f, 0.0f }, { 1.0f, 1.0f, 0.0f} };
        const Scarlet::Math::BoundingBox box;

        return !Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionOnTransformedBox()
    {
        const Scarlet::Math::Ray ray { { 1.0f, 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f} };
        Scarlet::Math::BoundingBox box;
        box.SetTranslation({0.0f, 1.0f, 0.0f});

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionOnTranslatedAndRotatedBox()
    {
        const Scarlet::Math::Ray ray { { -1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        Scarlet::Math::BoundingBox box { { 0.0f, -1.0f, -1.0f}, { 0.5f, 1.0f, 1.0f} };
        box.SetTranslation({1.0f, 0.0f, 0.0f });
        box.SetRotation(Scarlet::Math::Quat{ Scarlet::Math::Radians(90.f), 0.0f, 0.0f });

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(ray, box);
    }

    inline static bool BoundingBoxIntersectionOnRotatedBox()
    {
        const Scarlet::Math::Ray rayX { { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 0.0f} };
        const Scarlet::Math::Ray rayY { { -0.3f,  0.5f, 0.0f }, { 1.0f, 0.0f, 0.0f} };

        Scarlet::Math::BoundingBox box;
        box.SetTranslation({0.5f, 0.5f, 0.0f});
        box.SetRotation(Scarlet::Math::Quat{ Scarlet::Math::Radians(45.f), 0.0f, 0.0f });

        return Scarlet::Math::Ray::RayIntersectsWithBoundingBox(rayX, box) && Scarlet::Math::Ray::RayIntersectsWithBoundingBox(rayY, box);
    }
};

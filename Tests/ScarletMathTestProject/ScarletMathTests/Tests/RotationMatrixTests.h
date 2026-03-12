#pragma once

#include <ScarletTestTemplate/Core/TestRegistry.h>

#include <ScarletMath/Math.h>

class RotationMatrixTests
{
public:
    explicit inline RotationMatrixTests(Scarlet::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitXPointYawRotation", UnitXPointYawRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitXPointPitchRotation", UnitXPointPitchRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitXPointRollRotation", UnitXPointRollRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitYPointYawRotation", UnitYPointYawRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitYPointPitchRotation", UnitYPointPitchRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitYPointRollRotation", UnitYPointRollRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitZPointYawRotation", UnitZPointYawRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitZPointPitchRotation", UnitZPointPitchRotation);
        testRegistry->AddTestCase("RotationMatrix Tests", "UnitZPointRollRotation", UnitZPointRollRotation);
    }

    inline static bool RotatePointTest(const Scarlet::Math::Vec3 testPoint, const Scarlet::Math::Vec3 solution, 
                            const float yawDeg, const float pitchDeg, const float rollDeg)
    {
        const Scarlet::Math::Mat4 rotationMatrix     = Scarlet::Math::Trig::RotationMatrix(yawDeg, pitchDeg, rollDeg);
        const Scarlet::Math::Vec3 solutionFromMatrix = Scarlet::Math::Vec4{ testPoint, 0.0f } * rotationMatrix;

        return Scarlet::Math::IsEqual(solutionFromMatrix, solution);
    }

    inline static bool UnitXPointYawRotation()
    {
        return RotatePointTest({ 1.0f, 0.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 90.0f, 0.0f, 0.0f);
    }

    inline static bool UnitXPointPitchRotation()
    {
        return RotatePointTest({ 1.0f, 0.0f, 0.0f }, { 1.0f, 0.0f, 0.0f }, 0.0f, 90.0f, 0.0f);
    }

    inline static bool UnitXPointRollRotation()
    {
        return RotatePointTest({ 1.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, -1.0f }, 0.0f, 0.0f, 90.0f);
    }

    inline static bool UnitYPointYawRotation()
    {
        return RotatePointTest({ 0.0f, 1.0f, 0.0f }, { -1.0f, 0.0f, 0.0f }, 90.0f, 0.0f, 0.0f);
    }

    inline static bool UnitYPointPitchRotation()
    {
        return RotatePointTest({ 0.0f, 1.0f, 0.0f }, { 0.0f, 0.0f, 1.0f }, 0.0f, 90.0f, 0.0f);
    }

    inline static bool UnitYPointRollRotation()
    {
        return RotatePointTest({ 0.0f, 1.0f, 0.0f }, { 0.0f, 1.0f, 0.0f }, 0.0f, 0.0f, 90.0f);
    }

    inline static bool UnitZPointYawRotation()
    {
        return RotatePointTest({ 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, 90.0f, 0.0f, 0.0f);
    }

    inline static bool UnitZPointPitchRotation()
    {
        return RotatePointTest({ 0.0f, 0.0f, 1.0f }, { 0.0f, -1.0f, 0.0f }, 0.0f, 90.0f, 0.0f);
    }

    inline static bool UnitZPointRollRotation()
    {
        return RotatePointTest({ 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f, 0.0f }, 0.0f, 0.0f, 90.0f);
    }

    inline static bool RotationMatrixIsInOrderZXY()
    {
        // This is unfortunately a hardcoded test. This is just simply to verify that the matrices retain the order Z X Y.
        // The point was calculated using https://www.mathforengineers.com/math-calculators/3D-point-rotation-calculator.html.
        // This test could be changed to construct the matrices instead, however, this would make the test redundant as that is exactly
        // what Scarlet::Trig::RotateMatrix does. Therefore, to be more of an accurate test, the use of a hardcoded point is better.
        return RotatePointTest({ 1.0f, 2.0f, 3.0f }, { 1.683013f, -0.633975f, 3.281089f }, 90.0f, 30.0f, 60.0f);
    }
};

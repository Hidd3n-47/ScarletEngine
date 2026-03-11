#pragma once

#include <ScarletTestTemplate/Core/TestRegistry.h>

#include <ScarletMath/Quat.h>
#include <ScarletMath/Trig.h>

class QuaternionTests
{
public:
    explicit inline QuaternionTests(Scarlet::TestRegistry* testRegistry)
    {
        // Quaternions created correctly.
        testRegistry->AddTestCase("Quaternion Tests", "QuaternionReturnsCorrectRotationMatrixWhenNoRotation", QuaternionReturnsCorrectRotationMatrixWhenNoRotation);
        testRegistry->AddTestCase("Quaternion Tests", "PointUnitXRotatesCorrectly", PointUnitXRotatesCorrectly);
        testRegistry->AddTestCase("Quaternion Tests", "PointUnitYRotatesCorrectly", PointUnitYRotatesCorrectly);
        testRegistry->AddTestCase("Quaternion Tests", "PointUnitZRotatesCorrectly", PointUnitZRotatesCorrectly);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectly", PointRotatesCorrectly);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawAt90Degree", PointRotatesCorrectlyWithYawAt90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithPitchAt90Degree", PointRotatesCorrectlyWithPitchAt90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithRollAt90Degree", PointRotatesCorrectlyWithRollAt90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawPitchAt90Degree", PointRotatesCorrectlyWithYawPitchAt90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithPitchRollAt90Degree", PointRotatesCorrectlyWithPitchRollAt90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawPitchRollAt90Degree", PointRotatesCorrectlyWithYawPitchRollAt90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawAt180Degree", PointRotatesCorrectlyWithYawAt180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithPitchAt180Degree", PointRotatesCorrectlyWithPitchAt180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithRollAt180Degree", PointRotatesCorrectlyWithRollAt180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawPitchAt180Degree", PointRotatesCorrectlyWithYawPitchAt180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithPitchRollAt180Degree", PointRotatesCorrectlyWithPitchRollAt180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawPitchRollAt180Degree", PointRotatesCorrectlyWithYawPitchRollAt180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawAt270Degree", PointRotatesCorrectlyWithYawAt270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithPitchAt270Degree", PointRotatesCorrectlyWithPitchAt270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithRollAt270Degree", PointRotatesCorrectlyWithRollAt270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawPitchAt270Degree", PointRotatesCorrectlyWithYawPitchAt270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithPitchRollAt270Degree", PointRotatesCorrectlyWithPitchRollAt270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "PointRotatesCorrectlyWithYawPitchRollAt270Degree", PointRotatesCorrectlyWithYawPitchRollAt270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationYaw", ExtractedAnglesResultsInSameRotationYaw);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationPitch", ExtractedAnglesResultsInSameRotationPitch);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationRoll", ExtractedAnglesResultsInSameRotationRoll);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationYawPitch", ExtractedAnglesResultsInSameRotationYawPitch);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationPitchRoll", ExtractedAnglesResultsInSameRotationPitchRoll);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationYawPitchRoll", ExtractedAnglesResultsInSameRotationYawPitchRoll);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationYaw90Degree", ExtractedAnglesResultsInSameRotationYaw90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationPitch90Degree", ExtractedAnglesResultsInSameRotationPitch90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationRoll90Degree", ExtractedAnglesResultsInSameRotationRoll90Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationYaw180Degree", ExtractedAnglesResultsInSameRotationYaw180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationPitch180Degree", ExtractedAnglesResultsInSameRotationPitch180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationRoll180Degree", ExtractedAnglesResultsInSameRotationRoll180Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationYaw270Degree", ExtractedAnglesResultsInSameRotationYaw270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationPitch270Degree", ExtractedAnglesResultsInSameRotationPitch270Degree);
        testRegistry->AddTestCase("Quaternion Tests", "ExtractedAnglesResultsInSameRotationRoll270Degree", ExtractedAnglesResultsInSameRotationRoll270Degree);

    }

    inline static bool QuaternionReturnsCorrectRotationMatrixWhenNoRotation()
    {
        constexpr Scarlet::Math::Mat4 solution = {
            1.0, 0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, 0.0,
            0.0, 0.0, 1.0, 0.0,
            0.0, 0.0, 0.0, 1.0
        };

        const Scarlet::Math::Quat quat(0.0, 0.0, 0.0);

        return Scarlet::Math::IsEqual(solution, quat.GetRotationMatrix());
    }

    inline static bool TestPointQuaternionRotationMatchesMatrixRotation(const Scarlet::Math::Vec3 testPoint, 
                                                const float yawDeg, const float pitchDeg, const float rollDeg)
    {
        const float yawRad   = Scarlet::Math::Radians(yawDeg);
        const float pitchRad = Scarlet::Math::Radians(pitchDeg);
        const float rollRad  = Scarlet::Math::Radians(rollDeg);

        const Scarlet::Math::Mat4 rotationMatrix     = Scarlet::Math::Trig::RotationMatrix(yawDeg, pitchDeg, rollDeg);
        const Scarlet::Math::Vec3 solutionFromMatrix = Scarlet::Math::Vec4{ testPoint, 0.0f } * rotationMatrix;

        const Scarlet::Math::Quat quatFromYawPitchRoll{ yawRad, pitchRad, rollRad};
        const Scarlet::Math::Vec3 solutionFromQuat = Scarlet::Math::Quat::RotatePoint(testPoint, quatFromYawPitchRoll);

        return Scarlet::Math::IsEqual(solutionFromQuat, solutionFromMatrix);
    }

    inline static bool PointUnitXRotatesCorrectly()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 1.0f, 0.0f, 0.0f }, 4.7f, 47.0f, 147.0f);
    }

    inline static bool PointUnitYRotatesCorrectly()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 0.0f, 1.0f, 0.0f }, 4.7f, 47.0f, 147.0f);
    }

    inline static bool PointUnitZRotatesCorrectly()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 0.0f, 0.0f, 1.0f }, 4.7f, 47.0f, 147.0f);
    }

    inline static bool PointRotatesCorrectly()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 123.0f, 321.0f, 246.0f }, 4.7f, 47.0f, 147.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawAt90Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 90.0f, 0.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithPitchAt90Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 90.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithRollAt90Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 0.0f, 90.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawPitchAt90Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 90.0f, 90.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithPitchRollAt90Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 90.0f, 90.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawPitchRollAt90Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 90.0f, 90.0f, 90.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawAt180Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 180.0f, 0.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithPitchAt180Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 180.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithRollAt180Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 0.0f, 180.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawPitchAt180Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 180.0f, 180.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithPitchRollAt180Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 180.0f, 180.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawPitchRollAt180Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 180.0f, 180.0f, 180.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawAt270Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 270.0f, 0.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithPitchAt270Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 270.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithRollAt270Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 0.0f, 270.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawPitchAt270Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 270.0f, 270.0f, 0.0f);
    }

    inline static bool PointRotatesCorrectlyWithPitchRollAt270Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 0.0f, 270.0f, 270.0f);
    }

    inline static bool PointRotatesCorrectlyWithYawPitchRollAt270Degree()
    {
        return TestPointQuaternionRotationMatchesMatrixRotation({ 47.0f, 4.7f, 147.0f }, 270.0f, 270.0f, 270.0f);
    }

    inline static bool ExtractedAnglesResultInSameRotation(const Scarlet::Math::Vec3 testPoint, const float yawDeg, const float pitchDeg, const float rollDeg)
    {
        const float yawRad   = Scarlet::Math::Radians(yawDeg);
        const float pitchRad = Scarlet::Math::Radians(pitchDeg);
        const float rollRad  = Scarlet::Math::Radians(rollDeg);

        const Scarlet::Math::Quat quat{ yawRad, pitchRad, rollRad };
        const Scarlet::Math::Vec3 solutionFromQuat = Scarlet::Math::Quat::RotatePoint(testPoint, quat);

        double outYaw, outPitch, outRoll;
        quat.GetYawPitchRoll(outYaw, outPitch, outRoll);

        const Scarlet::Math::Mat4 rotationMatrix = Scarlet::Math::Trig::RotationMatrixRadians(outYaw, outPitch, outRoll);
        const Scarlet::Math::Vec3 solutionFromMatrix = Scarlet::Math::Vec4{ testPoint, 0.0f } *rotationMatrix;

        return Scarlet::Math::IsEqual(solutionFromQuat, solutionFromMatrix);
    }

    inline static bool ExtractedAnglesResultsInSameRotationYaw()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 47.0f, 0.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationPitch()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 47.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationRoll()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 0.0f, 47.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationYawPitch()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 147.0f, 741.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationPitchRoll()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 147.0f, 47.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationYawPitchRoll()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 47.0f, 147.0f, 23.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationYaw90Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 90.0f, 0.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationPitch90Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 90.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationRoll90Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 0.0f, 90.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationYaw180Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 180.0f, 0.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationPitch180Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 180.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationRoll180Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 0.0f, 180.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationYaw270Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 270.0f, 0.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationPitch270Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 270.0f, 0.0f);
    }

    inline static bool ExtractedAnglesResultsInSameRotationRoll270Degree()
    {
        return ExtractedAnglesResultInSameRotation({ 123.0f, 321.0f, 246.0f }, 0.0f, 0.0f, 270.0f);
    }
};

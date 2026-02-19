#pragma once

#include <stdexcept>

#include <ScarletTestTemplate/Core/TestRegistry.h>

#define SCARLENT_TEST
#include <ScarlEnt/Registry.h>
#include <ScarlEnt/Scene.h>

#include "SimpleTestTypes.h"

class RegistryTests
{
public:
    explicit inline RegistryTests(Scarlet::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("RegistryTests", "DoubleInitCausesAssert", DoubleInitCausesAssert);
        testRegistry->AddTestCase("RegistryTests", "RegisteringFirstComponentReturnsOne", RegisteringFirstComponentReturnsOne);
        testRegistry->AddTestCase("RegistryTests", "RegisteringSecondComponentReturnsTwo", RegisteringSecondComponentReturnsTwo);
        testRegistry->AddTestCase("RegistryTests", "NoActiveSceneReturnsAnInvalidHandle", NoActiveSceneReturnsAnInvalidHandle);
        testRegistry->AddTestCase("RegistryTests", "GettingRegisteredComponentReturnsSameId", GettingRegisteredComponentReturnsSameId);
        testRegistry->AddTestCase("RegistryTests", "CanCreateAndDestroyAScene", CanCreateAndDestroyAScene);
        testRegistry->AddTestCase("RegistryTests", "SettingSceneChangesCurrentlyActiveScene", SettingSceneChangesCurrentlyActiveScene);
        testRegistry->AddTestCase("RegistryTests", "IndexOfSceneUpdatesCorrectlyWhenRemovingAScene", IndexOfSceneUpdatesCorrectlyWhenRemovingAScene);
    }

    static bool DoubleInitCausesAssert()
    {
        bool passed = false;

        ScarlEnt::Registry::Init();

        try
        {
            ScarlEnt::Registry::Init();
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool RegisteringFirstComponentReturnsOne()
    {
        return ScarlEnt::Registry::Instance().GetOrRegisterComponentBit<Vec2>() == 0b1;
    }

    static bool RegisteringSecondComponentReturnsTwo()
    {
        return ScarlEnt::Registry::Instance().GetOrRegisterComponentBit<Vec3>() == 0b10;
    }

    static bool GettingRegisteredComponentReturnsSameId()
    {
        return ScarlEnt::Registry::Instance().GetOrRegisterComponentBit<Vec2>() == 0b1
            && ScarlEnt::Registry::Instance().GetOrRegisterComponentBit<Vec3>() == 0b10;
    }

    static bool NoActiveSceneReturnsAnInvalidHandle()
    {
        return !ScarlEnt::Registry::Instance().GetActiveScene().IsValid();
    }

    static bool CanCreateAndDestroyAScene()
    {
        bool passed = true;

        auto scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        passed &= ScarlEnt::Registry::Instance().GetNumberOfScenes() == 1;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        passed &= ScarlEnt::Registry::Instance().GetNumberOfScenes() == 0;

        return passed;
    }

    static bool SettingSceneChangesCurrentlyActiveScene()
    {
        bool passed = true;

        auto scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        passed &= !ScarlEnt::Registry::Instance().GetActiveScene().IsValid();
        passed &= ScarlEnt::Registry::Instance().GetNumberOfScenes() == 1;

        ScarlEnt::Registry::Instance().SetActiveScene(scene);

        auto activeScene = ScarlEnt::Registry::Instance().GetActiveScene();
        passed &= activeScene.IsValid();
        passed &= activeScene == scene;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        passed &= ScarlEnt::Registry::Instance().GetNumberOfScenes() == 0;

        return passed;
    }

    static bool IndexOfSceneUpdatesCorrectlyWhenRemovingAScene()
    {
        bool passed = true;

        auto scene1 = ScarlEnt::Registry::Instance().CreateScene("Testing1");
        auto scene2 = ScarlEnt::Registry::Instance().CreateScene("Testing2");

        passed &= scene1->GetRegistryIndex() == 0;
        passed &= scene2->GetRegistryIndex() == 1;

        ScarlEnt::Registry::Instance().RemoveScene(scene1);

        passed &= scene2->GetRegistryIndex() == 0;

        ScarlEnt::Registry::Instance().RemoveScene(scene2);

        return passed;
    }
};

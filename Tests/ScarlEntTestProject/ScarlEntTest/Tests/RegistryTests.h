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
        testRegistry->AddTestCase("RegistryTests", "AssertsWhenTryingToRemoveASceneThatIsNotValid", AssertsWhenTryingToRemoveASceneThatIsNotValid);
        testRegistry->AddTestCase("RegistryTests", "AssertWhenTryingToGetComponentIdOfNonRegisteredComponent", AssertWhenTryingToGetComponentIdOfNonRegisteredComponent);
        testRegistry->AddTestCase("RegistryTests", "GetTheCorrectIdFromComponentBitset", GetTheCorrectIdFromComponentBitset);
        testRegistry->AddTestCase("RegistryTests", "AssertsWhenTryingToChangeSceneAtRuntimeThroughSetActiveScene", AssertsWhenTryingToChangeSceneAtRuntimeThroughSetActiveScene);
    }

    static bool DoubleInitCausesAssert()
    {
        bool passed = false;

        // Terminate in case it has been initialised by other tests. If it hasn't, terminating will do nothing so it's safe.
        ScarlEnt::Registry::Terminate();
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
        return ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec2>().bitmask == 0b1;
    }

    static bool RegisteringSecondComponentReturnsTwo()
    {
        return ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec3>().bitmask == 0b10;
    }

    static bool GettingRegisteredComponentReturnsSameId()
    {
        return ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec2>().bitmask == 0b1
            && ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec3>().bitmask == 0b10;
    }

    static bool NoActiveSceneReturnsAnInvalidHandle()
    {
        return !ScarlEnt::Registry::Instance().GetActiveScene().IsValid();
    }

    static bool CanCreateAndDestroyAScene()
    {
        bool passed = true;

        auto scene = ScarlEnt::Registry::Instance().GetOrCreateScene("Testing");

        passed &= ScarlEnt::Registry::Instance().GetNumberOfScenes() == 1;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        passed &= ScarlEnt::Registry::Instance().GetNumberOfScenes() == 0;

        return passed;
    }

    static bool SettingSceneChangesCurrentlyActiveScene()
    {
        bool passed = true;

        auto scene = ScarlEnt::Registry::Instance().GetOrCreateScene("Testing");

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

    static bool AssertsWhenTryingToRemoveASceneThatIsNotValid()
    {
        bool passed = false;

        const std::string sceneName{ "Testing" };

        auto scene = ScarlEnt::Registry::Instance().GetOrCreateScene(sceneName);
        ScarlEnt::Registry::Instance().RemoveScene(scene);

        try
        {
            ScarlEnt::Registry::Instance().RemoveScene(scene);
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool AssertWhenTryingToGetComponentIdOfNonRegisteredComponent()
    {
        bool passed = false;

        try
        {
            (void)ScarlEnt::Registry::Instance().GetComponentIdFromBitmask(0b1000);
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool GetTheCorrectIdFromComponentBitset()
    {
        bool passed = true;

        const ScarlEnt::ComponentId componentId = ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec2>();

        passed &= ScarlEnt::Registry::Instance().GetComponentIdFromBitmask(componentId.bitmask) == componentId.id;

        return passed;
    }

    static bool AssertsWhenTryingToChangeSceneAtRuntimeThroughSetActiveScene()
    {
        bool passed = false;

        Scarlet::WeakHandle<ScarlEnt::Scene> test1 = ScarlEnt::Registry::Instance().GetOrCreateScene("Test1");
        Scarlet::WeakHandle<ScarlEnt::Scene> test2 = ScarlEnt::Registry::Instance().GetOrCreateScene("Test2");

        ScarlEnt::Registry::Instance().SetActiveScene(test1);

        try
        {
            ScarlEnt::Registry::Instance().SetActiveScene(test2);
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }
};

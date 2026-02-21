#pragma once

#include <stdexcept>

#include <ScarletTestTemplate/Core/TestRegistry.h>

#include <ScarlEnt/Scene.h>

#include "SimpleTestTypes.h"

class SceneTests
{
public:
    explicit inline SceneTests(Scarlet::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("SceneTests", "AddedComponentWithEntityHasCorrectInitialValuesTest1", AddedComponentWithEntityHasCorrectInitialValuesTest1);
        testRegistry->AddTestCase("SceneTests", "AddedComponentWithEntityHasCorrectInitialValuesTest2", AddedComponentWithEntityHasCorrectInitialValuesTest2);
        testRegistry->AddTestCase("SceneTests", "AddedComponentWithEntityHasCorrectInitialValuesTest3", AddedComponentWithEntityHasCorrectInitialValuesTest3);
        testRegistry->AddTestCase("SceneTests", "AddedComponentWithEntityHasCorrectInitialValuesMultipleTypes", AddedComponentWithEntityHasCorrectInitialValuesMultipleTypes);
        testRegistry->AddTestCase("SceneTests", "AssertsWhenTryingToRemoveAnEntityThatIsNotValid", AssertsWhenTryingToRemoveAnEntityThatIsNotValid);
        testRegistry->AddTestCase("SceneTests", "FirstComponentsCorrectWhenRemovingSecondEntity", FirstComponentsCorrectWhenRemovingSecondEntity);
        testRegistry->AddTestCase("SceneTests", "SecondComponentsCorrectWhenRemovingFirstEntity", SecondComponentsCorrectWhenRemovingFirstEntity);
        testRegistry->AddTestCase("SceneTests", "MiddleComponentsCorrectWhenRemovingFirstAndSecondEntity", MiddleComponentsCorrectWhenRemovingFirstAndSecondEntity);
        testRegistry->AddTestCase("SceneTests", "MiddleComponentsCorrectWhenRemovingFirstAndSecondEntityComplexTypes", MiddleComponentsCorrectWhenRemovingFirstAndSecondEntityComplexTypes);

        ScarlEnt::Registry::Terminate();
        ScarlEnt::Registry::Init();
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesTest1()
    {
        constexpr int solution = 47;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId = scene->AddEntity<int>(static_cast<int>(solution));

        const bool passed = *scene->GetComponent<int, int>(entityId) == solution;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesTest2()
    {
        constexpr int solution = 47;
        constexpr int solutionSqr = solution * solution;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId = scene->AddEntity<Vec2>(Vec2{ solution, solutionSqr });

        const bool passed = *scene->GetComponent<Vec2, Vec2>(entityId) == Vec2{ solution, solutionSqr };

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesTest3()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId = scene->AddEntity<Vec3>(Vec3{ solution, solutionSqr, solutionCbd });

        const bool passed = *scene->GetComponent<Vec3, Vec3>(entityId) == Vec3{ solution, solutionSqr, solutionCbd};

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesMultipleTypes()
    {
        bool passed = true;

        constexpr int solution = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solution), Vec2{ solution, solutionSqr }, Vec3{ solution, solutionSqr, solutionCbd });

        passed &= *scene->GetComponent<int , int, Vec2, Vec3>(entityId) == solution;
        passed &= *scene->GetComponent<Vec2, int, Vec2, Vec3>(entityId) == Vec2{ solution, solutionSqr};
        passed &= *scene->GetComponent<Vec3, int, Vec2, Vec3>(entityId) == Vec3{ solution, solutionSqr, solutionCbd };

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AssertsWhenTryingToRemoveAnEntityThatIsNotValid()
    {
        bool passed = false;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId = scene->AddEntity<int>(0);
        scene->RemoveEntity<int>(entityId);

        try
        {
            scene->RemoveEntity<int>(entityId);
        }
        catch (std::runtime_error&)
        {
            passed = true;
        }

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool FirstComponentsCorrectWhenRemovingSecondEntity()
    {
        constexpr int solution = 47;
        constexpr int solutionSqr = solution * solution;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId1 = scene->AddEntity<Vec2>(Vec2{ solution, solution });
        const Scarlet::Ulid entityId2 = scene->AddEntity<Vec2>(Vec2{solutionSqr, solutionSqr });

        scene->RemoveEntity<Vec2>(entityId2);

        Scarlet::WeakHandle<Vec2> component = scene->GetComponent<Vec2, Vec2>(entityId1);
        passed &= component->x == solution;
        passed &= component->y == solution;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool SecondComponentsCorrectWhenRemovingFirstEntity()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId1 = scene->AddEntity<Vec2>(Vec2{ solution, solution });
        const Scarlet::Ulid entityId2 = scene->AddEntity<Vec2>(Vec2{ solutionSqr, solutionSqr });

        scene->RemoveEntity<Vec2>(entityId1);

        Scarlet::WeakHandle<Vec2> component = scene->GetComponent<Vec2, Vec2>(entityId2);
        passed &= component->x == solutionSqr;
        passed &= component->y == solutionSqr;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool MiddleComponentsCorrectWhenRemovingFirstAndSecondEntity()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId1 = scene->AddEntity<Vec2>(Vec2{ solution, solution });
        const Scarlet::Ulid entityId2 = scene->AddEntity<Vec2>(Vec2{ solutionSqr, solutionSqr });
        const Scarlet::Ulid entityId3 = scene->AddEntity<Vec2>(Vec2{ solutionCbd, solutionCbd });

        scene->RemoveEntity<Vec2>(entityId1);
        scene->RemoveEntity<Vec2>(entityId3);

        Scarlet::WeakHandle<Vec2> component = scene->GetComponent<Vec2, Vec2>(entityId2);
        passed &= component->x == solutionSqr;
        passed &= component->y == solutionSqr;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool MiddleComponentsCorrectWhenRemovingFirstAndSecondEntityComplexTypes()
    {
        constexpr int solution = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const Scarlet::Ulid entityId1 = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solution), Vec2{ solution, solution }, Vec3{ solution, solution, solution });
        const Scarlet::Ulid entityId2 = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solutionSqr), Vec2{ solutionSqr, solutionSqr }, Vec3{ solutionSqr, solutionSqr, solutionSqr });
        const Scarlet::Ulid entityId3 = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solutionCbd), Vec2{ solutionCbd, solutionCbd }, Vec3{ solutionCbd, solutionCbd, solutionCbd });

        scene->RemoveEntity<int, Vec2, Vec3>(entityId1);
        scene->RemoveEntity<int, Vec2, Vec3>(entityId3);

        passed &= *scene->GetComponent<int, int, Vec2, Vec3>(entityId2) == solutionSqr;

        Scarlet::WeakHandle<Vec2> component = scene->GetComponent<Vec2, int, Vec2, Vec3>(entityId2);
        passed &= component->x == solutionSqr;
        passed &= component->y == solutionSqr;

        Scarlet::WeakHandle<Vec3> component2 = scene->GetComponent<Vec3, int, Vec2, Vec3>(entityId2);
        passed &= component2->x == solutionSqr;
        passed &= component2->y == solutionSqr;
        passed &= component2->z == solutionSqr;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }
};

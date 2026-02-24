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
        testRegistry->AddTestCase("SceneTests", "SceneUpdateChangedEntitiesCorrectly", SceneUpdateChangedEntitiesCorrectly);
        testRegistry->AddTestCase("SceneTests", "SceneUpdateIteratesThroughAllArchetypesWithSameSubset", SceneUpdateIteratesThroughAllArchetypesWithSameSubset);
        testRegistry->AddTestCase("SceneTests", "SceneUpdateChangedEntitiesCorrectlyWithMultipleComponents", SceneUpdateChangedEntitiesCorrectlyWithMultipleComponents);

        ScarlEnt::Registry::Terminate();
        ScarlEnt::Registry::Init();
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesTest1()
    {
        constexpr int solution = 47;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity = scene->AddEntity<int>(static_cast<int>(solution));

        const bool passed = entity.GetComponent<int>() == solution;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesTest2()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity = scene->AddEntity<Vec2>(Vec2{ solution, solutionSqr });

        const bool passed = entity.GetComponent<Vec2>() == Vec2{ solution, solutionSqr };

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesTest3()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity = scene->AddEntity<Vec3>(Vec3{ solution, solutionSqr, solutionCbd });

        const bool passed = entity.GetComponent<Vec3>() == Vec3{ solution, solutionSqr, solutionCbd};

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AddedComponentWithEntityHasCorrectInitialValuesMultipleTypes()
    {
        bool passed = true;

        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solution), Vec2{ solution, solutionSqr }, Vec3{ solution, solutionSqr, solutionCbd });

        passed &= entity.GetComponent<int>()  == solution;
        passed &= entity.GetComponent<Vec2>() == Vec2{ solution, solutionSqr};
        passed &= entity.GetComponent<Vec3>() == Vec3{ solution, solutionSqr, solutionCbd };

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool AssertsWhenTryingToRemoveAnEntityThatIsNotValid()
    {
        bool passed = false;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        const ScarlEnt::EntityHandle entity = scene->AddEntity<int>(0);
        scene->RemoveEntity(entity);

        try
        {
            scene->RemoveEntity(entity);
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
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<Vec2>(Vec2{ solution, solution });
        ScarlEnt::EntityHandle entity2 = scene->AddEntity<Vec2>(Vec2{solutionSqr, solutionSqr });

        scene->RemoveEntity(entity2);

        const Vec2& component = entity1.GetComponent<Vec2>();
        passed &= component.x == solution;
        passed &= component.y == solution;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool SecondComponentsCorrectWhenRemovingFirstEntity()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<Vec2>(Vec2{ solution, solution });
        ScarlEnt::EntityHandle entity2 = scene->AddEntity<Vec2>(Vec2{ solutionSqr, solutionSqr });

        scene->RemoveEntity(entity1);

        const Vec2& component = entity2.GetComponent<Vec2>();
        passed &= component.x == solutionSqr;
        passed &= component.y == solutionSqr;

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

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<Vec2>(Vec2{ solution, solution });
        ScarlEnt::EntityHandle entity2 = scene->AddEntity<Vec2>(Vec2{ solutionSqr, solutionSqr });
        ScarlEnt::EntityHandle entity3 = scene->AddEntity<Vec2>(Vec2{ solutionCbd, solutionCbd });

        scene->RemoveEntity(entity1);
        scene->RemoveEntity(entity3);

        const Vec2& component = entity2.GetComponent<Vec2>();
        passed &= component.x == solutionSqr;
        passed &= component.y == solutionSqr;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool MiddleComponentsCorrectWhenRemovingFirstAndSecondEntityComplexTypes()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solution), Vec2{ solution, solution }, Vec3{ solution, solution, solution });
        ScarlEnt::EntityHandle entity2 = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solutionSqr), Vec2{ solutionSqr, solutionSqr }, Vec3{ solutionSqr, solutionSqr, solutionSqr });
        ScarlEnt::EntityHandle entity3 = scene->AddEntity<int, Vec2, Vec3>(static_cast<int>(solutionCbd), Vec2{ solutionCbd, solutionCbd }, Vec3{ solutionCbd, solutionCbd, solutionCbd });

        scene->RemoveEntity(entity1);
        scene->RemoveEntity(entity3);

        passed &= entity2.GetComponent<int>() == solutionSqr;

        const Vec2& component = entity2.GetComponent<Vec2>();
        passed &= component.x == solutionSqr;
        passed &= component.y == solutionSqr;

        const Vec3& component2 = entity2.GetComponent<Vec3>();
        passed &= component2.x == solutionSqr;
        passed &= component2.y == solutionSqr;
        passed &= component2.z == solutionSqr;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool SceneUpdateChangedEntitiesCorrectly()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<Vec3>( Vec3{ solution, solutionSqr, solutionCbd });

        scene->RegisterSystem<Vec3>([](Vec3& vector) { vector.x *= 2; vector.y *= 3; vector.z *= 4; });

        scene->Update();

        const Vec3& component = entity1.GetComponent<Vec3>();
        passed &= component.x == solution    * 2;
        passed &= component.y == solutionSqr * 3;
        passed &= component.z == solutionCbd * 4;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool SceneUpdateIteratesThroughAllArchetypesWithSameSubset()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<Vec3>(Vec3{ solution, solutionSqr, solutionCbd });
        ScarlEnt::EntityHandle entity2 = scene->AddEntity<Vec2, Vec3>(Vec2{ solution, solution }, Vec3{ solution, solutionSqr, solutionCbd });

        scene->RegisterSystem<Vec3>([](Vec3& vector) { vector.x *= 2; vector.y *= 3; vector.z *= 4; });

        scene->Update();

        auto CheckComponent = [](bool& passed, const Vec3& component) {
                passed &= component.x == solution    * 2;
                passed &= component.y == solutionSqr * 3;
                passed &= component.z == solutionCbd * 4;
            };

        CheckComponent(passed, entity1.GetComponent<Vec3>());
        CheckComponent(passed, entity2.GetComponent<Vec3>());

        const Vec2& otherComponent = entity2.GetComponent<Vec2>();
        passed &= otherComponent.x == solution;
        passed &= otherComponent.y == solution;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool SceneUpdateChangedEntitiesCorrectlyWithMultipleComponents()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::EntityHandle entity1 = scene->AddEntity<Vec2, Vec3>(Vec2{ solution, solution }, Vec3{ solution, solutionSqr, solutionCbd });

        scene->RegisterSystem<Vec2, Vec3>([](Vec2& v2, Vec3& v3) {
                v2.x *= v2.x;
                v2.y *= v2.y;

                v3.x *= 2;
                v3.y *= 3;
                v3.z *= 4;
            });

        scene->Update();

        const Vec2& comp1 = entity1.GetComponent<Vec2>();
        const Vec3& comp2 = entity1.GetComponent<Vec3>();

        passed &= comp1.x == solutionSqr;
        passed &= comp1.y == solutionSqr;

        passed &= comp2.x == solution    * 2;
        passed &= comp2.y == solutionSqr * 3;
        passed &= comp2.z == solutionCbd * 4;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }
};

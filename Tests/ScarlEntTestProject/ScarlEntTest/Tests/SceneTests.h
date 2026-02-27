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
        testRegistry->AddTestCase("SceneTests", "AddingMutableEntityHasRuntimeIdOfZero", AddingMutableEntityHasRuntimeIdOfZero);
        testRegistry->AddTestCase("SceneTests", "AddingMultipleMutableEntityHasCorrectRuntimeIds", AddingMultipleMutableEntityHasCorrectRuntimeIds);
        testRegistry->AddTestCase("SceneTests", "MutableEntityAddedToComponentBitsetWithNoComponents", MutableEntityAddedToComponentBitsetWithNoComponents);
        testRegistry->AddTestCase("SceneTests", "MutableEntityAddedToComponentBitsetWhenAddingAComponent", MutableEntityAddedToComponentBitsetWhenAddingAComponent);
        testRegistry->AddTestCase("SceneTests", "MutableEntityAddedToComponentBitsetWhenAddingMultipleComponent", MutableEntityAddedToComponentBitsetWhenAddingMultipleComponent);
        testRegistry->AddTestCase("SceneTests", "MutableEntityCanGetAddedComponent", MutableEntityCanGetAddedComponent);
        testRegistry->AddTestCase("SceneTests", "MutableEntityCanGetAddedComponentsMultiple", MutableEntityCanGetAddedComponentsMultiple);
        testRegistry->AddTestCase("SceneTests", "ChangingMutableEntityComponentCorrectlyUpdatesTheComponent", ChangingMutableEntityComponentCorrectlyUpdatesTheComponent);
        testRegistry->AddTestCase("SceneTests", "AssertsWhenTryingToRemoveNonAddedComponent", AssertsWhenTryingToRemoveNonAddedComponent);
        testRegistry->AddTestCase("SceneTests", "HasComponentIsFalseWithNoComponents", HasComponentIsFalseWithNoComponents);
        testRegistry->AddTestCase("SceneTests", "HasComponentTrueAfterAddingComponent", HasComponentTrueAfterAddingComponent);
        testRegistry->AddTestCase("SceneTests", "HasComponentFalseAfterRemovingComponent", HasComponentFalseAfterRemovingComponent);
        testRegistry->AddTestCase("SceneTests", "CanRemoveComponentAdded", CanRemoveComponentAdded);
        testRegistry->AddTestCase("SceneTests", "AssertWhenTryingToRemoveComponentTwice", AssertWhenTryingToRemoveComponentTwice);
        testRegistry->AddTestCase("SceneTests", "MutableEntityUpdatesWhenSceneUpdates", MutableEntityUpdatesWhenSceneUpdates);
        testRegistry->AddTestCase("SceneTests", "SceneUpdateIteratesThroughAllSparseSetsWithSameSubset", SceneUpdateIteratesThroughAllSparseSetsWithSameSubset);
        testRegistry->AddTestCase("SceneTests", "SceneUpdateChangedEntitiesCorrectlyWithMultipleComponentsSparseSet", SceneUpdateChangedEntitiesCorrectlyWithMultipleComponentsSparseSet);
        testRegistry->AddTestCase("SceneTests", "BothEntitiesAndMutableEntityUpdatesWhenSceneUpdates", BothEntitiesAndMutableEntityUpdatesWhenSceneUpdates);
        testRegistry->AddTestCase("SceneTests", "BothMutableAndEntitiesUpdatesWhenSceneUpdatesMultipleComponents", BothMutableAndEntitiesUpdatesWhenSceneUpdatesMultipleComponents);
        testRegistry->AddTestCase("SceneTests", "MutableEntitiesUpdateWhenChangingComponentComposition", MutableEntitiesUpdateWhenChangingComponentComposition);
        testRegistry->AddTestCase("SceneTests", "NoAssertsWhenRemovingMutableEntity", NoAssertsWhenRemovingMutableEntity);
        testRegistry->AddTestCase("SceneTests", "NoAssertsWhenRemovingMutableEntityWithComponents", NoAssertsWhenRemovingMutableEntityWithComponents);
        testRegistry->AddTestCase("SceneTests", "RemovesComponentsOnMutableEntityWhenRemoved", RemovesComponentsOnMutableEntityWhenRemoved);

#ifdef DEV_CONFIGURATION
        testRegistry->AddTestCase("SceneTests", "AssertsWhenTryingToUseMutableEntityAfterRemovalInDebugMode", AssertsWhenTryingToUseMutableEntityAfterRemovalInDebugMode);
#endif // DEV_CONFIGURATION.

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

    static bool AddingMutableEntityHasRuntimeIdOfZero()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        passed &= handle.GetId().runtimeId == 0;

        return passed;
    }

    static bool AddingMultipleMutableEntityHasCorrectRuntimeIds()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle0 = scene->AddMutableEntity();
        ScarlEnt::MutableEntityHandle handle1 = scene->AddMutableEntity();
        ScarlEnt::MutableEntityHandle handle2 = scene->AddMutableEntity();

        passed &= handle0.GetId().runtimeId == 0;
        passed &= handle1.GetId().runtimeId == 1;
        passed &= handle2.GetId().runtimeId == 2;

        return passed;
    }

    static bool MutableEntityAddedToComponentBitsetWithNoComponents()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        passed &= handle.GetComponentBitset() == 0;

        return passed;
    }

    static bool MutableEntityAddedToComponentBitsetWhenAddingAComponent()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>();

        passed &= handle.GetComponentBitset() == ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec2>().bitmask;

        return passed;
    }

    static bool MutableEntityAddedToComponentBitsetWhenAddingMultipleComponent()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>();
        handle.AddComponent<Vec3>();

        const uint64 correctBiset = ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec2>().bitmask | ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec3>().bitmask;
        passed &= handle.GetComponentBitset() == correctBiset;

        return passed;
    }

    static bool MutableEntityCanGetAddedComponent()
    {
        constexpr int number   = 47;
        constexpr int numberSq = number * number;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>(number, numberSq);

        passed &= handle.GetComponent<Vec2>() == Vec2{ number, numberSq };

        return passed;
    }

    static bool MutableEntityCanGetAddedComponentsMultiple()
    {
        constexpr int number   = 47;
        constexpr int numberSq = number   * number;
        constexpr int numberCb = numberSq * number;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<int>(number);
        handle.AddComponent<Vec2>(number, numberSq);
        handle.AddComponent<Vec3>(number, numberSq, numberCb);

        // Purposefully testing different order to ensure it's not ordered based.
        passed &= handle.GetComponent<int>()  == number;
        passed &= handle.GetComponent<Vec3>() == Vec3{ number, numberSq, numberCb };
        passed &= handle.GetComponent<Vec2>() == Vec2{ number, numberSq };

        return passed;
    }

    static bool ChangingMutableEntityComponentCorrectlyUpdatesTheComponent()
    {
        constexpr int number   = 47;
        constexpr int numberSq = number * number;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>(number, numberSq);

        handle.GetComponent<Vec2>() += Vec2{ number , numberSq };

        passed &= handle.GetComponent<Vec2>() == Vec2{ number * 2, numberSq * 2 };

        return passed;
    }

    static bool AssertsWhenTryingToRemoveNonAddedComponent()
    {
        bool passed = false;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        try
        {
            handle.RemoveComponent<Vec2>();
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }


    static bool HasComponentIsFalseWithNoComponents()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        passed &= !handle.HasComponent<Vec2>();

        return passed;
    }

    static bool HasComponentTrueAfterAddingComponent()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>();

        passed &= handle.HasComponent<Vec2>();

        return passed;
    }

    static bool HasComponentFalseAfterRemovingComponent()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>();
        handle.RemoveComponent<Vec2>();

        passed &= !handle.HasComponent<Vec2>();

        return passed;
    }

    static bool CanRemoveComponentAdded()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>();
        passed &= handle.HasComponent<Vec2>();

        handle.RemoveComponent<Vec2>();
        passed &= !handle.HasComponent<Vec2>();

        return passed;
    }

    static bool AssertWhenTryingToRemoveComponentTwice()
    {
        bool passed = false;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        handle.AddComponent<Vec2>();
        handle.RemoveComponent<Vec2>();

        try
        {
            handle.RemoveComponent<Vec2>();
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool MutableEntityUpdatesWhenSceneUpdates()
    {
        constexpr int number    = 47;
        constexpr int numberSqr = number * number;
        constexpr int numberCbd = number * numberSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle handle = scene->AddMutableEntity();

        scene->RegisterSystem<Vec3>([](Vec3& vector) { vector.x *= 2; vector.y *= 3; vector.z *= 4; });

        handle.AddComponent<Vec3>(number, numberSqr, numberCbd);

        scene->Update();

        Vec3& component = handle.GetComponent<Vec3>();

        passed &= component.x == number    * 2;
        passed &= component.y == numberSqr * 3;
        passed &= component.z == numberCbd * 4;

        return passed;
    }

    static bool SceneUpdateIteratesThroughAllSparseSetsWithSameSubset()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle entity1 = scene->AddMutableEntity();
        entity1.AddComponent<Vec3>(solution, solutionSqr, solutionCbd);

        ScarlEnt::MutableEntityHandle entity2 = scene->AddMutableEntity();
        entity2.AddComponent<Vec2>(solution, solution);
        entity2.AddComponent<Vec3>(solution, solutionSqr, solutionCbd);

        scene->RegisterSystem<Vec3>([](Vec3& vector) { vector.x *= 2; vector.y *= 3; vector.z *= 4; });

        scene->Update();

        auto CheckComponent = [](bool& passed, const Vec3& component) {
            passed &= component.x == solution * 2;
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

    static bool SceneUpdateChangedEntitiesCorrectlyWithMultipleComponentsSparseSet()
    {
        constexpr int solution    = 47;
        constexpr int solutionSqr = solution * solution;
        constexpr int solutionCbd = solution * solutionSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");

        ScarlEnt::MutableEntityHandle entity1 = scene->AddMutableEntity();

        entity1.AddComponent<Vec2>(solution, solution);
        entity1.AddComponent<Vec3>(solution, solutionSqr, solutionCbd);

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

        passed &= comp2.x == solution * 2;
        passed &= comp2.y == solutionSqr * 3;
        passed &= comp2.z == solutionCbd * 4;

        ScarlEnt::Registry::Instance().RemoveScene(scene);

        return passed;
    }

    static bool BothEntitiesAndMutableEntityUpdatesWhenSceneUpdates()
    {
        constexpr int number    = 47;
        constexpr int numberSqr = number * number;
        constexpr int numberCbd = number * numberSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene  = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::EntityHandle        staticEntity  = scene->AddEntity<Vec3>(Vec3{ number, numberSqr, numberCbd });
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        scene->RegisterSystem<Vec3>([](Vec3& vector) { vector.x *= 2; vector.y *= 3; vector.z *= 4; });

        mutableEntity.AddComponent<Vec3>(number, numberSqr, numberCbd);

        scene->Update();

        Vec3& componentMutable = mutableEntity.GetComponent<Vec3>();

        passed &= componentMutable.x == number * 2;
        passed &= componentMutable.y == numberSqr * 3;
        passed &= componentMutable.z == numberCbd * 4;


        Vec3& componentStatic = staticEntity.GetComponent<Vec3>();

        passed &= componentStatic.x == number * 2;
        passed &= componentStatic.y == numberSqr * 3;
        passed &= componentStatic.z == numberCbd * 4;

        return passed;
    }

    static bool BothMutableAndEntitiesUpdatesWhenSceneUpdatesMultipleComponents()
    {
        constexpr int number = 47;
        constexpr int numberSqr = number * number;
        constexpr int numberCbd = number * numberSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::EntityHandle        staticEntity = scene->AddEntity<Vec2, Vec3>(Vec2{ number, number }, Vec3{ number, numberSqr, numberCbd });
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        scene->RegisterSystem<Vec2, Vec3>([](Vec2& v2, Vec3& v3) {
            v2.x *= v2.x;
            v2.y *= v2.y;

            v3.x *= 2;
            v3.y *= 3;
            v3.z *= 4;
            });

        mutableEntity.AddComponent<Vec2>(number, number);
        mutableEntity.AddComponent<Vec3>(number, numberSqr, numberCbd);

        scene->Update();

        {

            const Vec2& comp1 = staticEntity.GetComponent<Vec2>();
            const Vec3& comp2 = staticEntity.GetComponent<Vec3>();

            passed &= comp1.x == numberSqr;
            passed &= comp1.y == numberSqr;

            passed &= comp2.x == number * 2;
            passed &= comp2.y == numberSqr * 3;
            passed &= comp2.z == numberCbd * 4;
        }

        {
            const Vec2& comp1 = mutableEntity.GetComponent<Vec2>();
            const Vec3& comp2 = mutableEntity.GetComponent<Vec3>();

            passed &= comp1.x == numberSqr;
            passed &= comp1.y == numberSqr;

            passed &= comp2.x == number * 2;
            passed &= comp2.y == numberSqr * 3;
            passed &= comp2.z == numberCbd * 4;
        }

        return passed;
    }

    static bool MutableEntitiesUpdateWhenChangingComponentComposition()
    {
        constexpr int number = 47;
        constexpr int numberSqr = number * number;
        constexpr int numberCbd = number * numberSqr;

        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        scene->RegisterSystem<Vec2, Vec3>([](Vec2& v2, Vec3& v3) { v3.x *= v2.x; v3.y *= v2.y; });
        scene->RegisterSystem<Vec3>([](Vec3& vector) { vector.x *= 2; vector.y *= 3; vector.z *= 4; });

        mutableEntity.AddComponent<Vec2>(number, number);
        mutableEntity.AddComponent<Vec3>(number, numberSqr, numberCbd);

        scene->Update();
        // Scene update should get mutable entity and update the vec3 by multiplying by vec2.
        // Since the entity contains a vec3 each vector component is going to be multiplied by 2, 3, and 4 respectively.

        mutableEntity.RemoveComponent<Vec2>();

        scene->Update();
        // Scene update should only multiply each vec3 vector component by 2, 3, and 4 respectively.

        const Vec3& comp2 = mutableEntity.GetComponent<Vec3>();

        // The first system multiplied v3.x * v2.x (so number * number), next system doubles the x value. The next update only doubles the x value.
        passed &= comp2.x == number * number * 2 * 2;
        // The first system multiplied v3.y * v2.y (so number * number), next system triples the y value. The next update only triples the y value.
        passed &= comp2.y == numberSqr * number * 3 * 3;
        // The first system leaves z untouched, next system quadruples the z value. The next update only quadruples the z value.
        passed &= comp2.z == numberCbd * 4 * 4;

        return passed;
    }

    static bool NoAssertsWhenRemovingMutableEntity()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene  = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        mutableEntity.DestroyEntity();

        return passed;
    }

    static bool NoAssertsWhenRemovingMutableEntityWithComponents()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene  = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        mutableEntity.AddComponent<Vec2>();
        mutableEntity.AddComponent<Vec3>();

        mutableEntity.DestroyEntity();

        return passed;
    }

    static bool RemovesComponentsOnMutableEntityWhenRemoved()
    {
        bool passed = true;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        auto componentManager = mutableEntity.GetComponentManagerRef();

        mutableEntity.AddComponent<Vec2>();
        mutableEntity.AddComponent<Vec3>();

        mutableEntity.DestroyEntity();

        auto componentArrays = componentManager->GetMutableComponentArrays();

        const ScarlEnt::ComponentId vec2Id = ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec2>();
        const ScarlEnt::ComponentId vec3Id = ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Vec3>();

        passed &= static_cast<ScarlEnt::ComponentManager::MutableComponentArray<Vec2>*>(componentArrays[vec2Id.id])->GetDense().size() == 0;
        passed &= static_cast<ScarlEnt::ComponentManager::MutableComponentArray<Vec3>*>(componentArrays[vec3Id.id])->GetDense().size() == 0;

        return passed;
    }

#ifdef DEV_CONFIGURATION
    static bool AssertsWhenTryingToUseMutableEntityAfterRemovalInDebugMode()
    {
        bool passed = false;

        Scarlet::WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().CreateScene("Testing");
        ScarlEnt::MutableEntityHandle mutableEntity = scene->AddMutableEntity();

        mutableEntity.DestroyEntity();

        try
        {
            mutableEntity.AddComponent<Vec2>();
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }
#endif // DEV_CONFIGURATION.

    //todo it would be good to test the sets as that is the part I am most not sure of, however not sure a nice way to get access to it.
    //todo make the unordered_set as a ScarletCore.
};

#pragma once

#include <ScarletTestTemplate/Core/TestRegistry.h>

#include <ScarlEnt/Archetype.h>

#include "SimpleTestTypes.h"

class ArchetypeTests
{
public:
    explicit inline ArchetypeTests(Scarlet::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("ArchetypeTest", "SinglePrimitiveTypeInitialisedCorrectly", SinglePrimitiveTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "SingleCustomTypeInitialisedCorrectly", SingleCustomTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "DoublePrimitiveTypeInitialisedCorrectly", DoublePrimitiveTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "DoubleCustomTypeInitialisedCorrectly", DoubleCustomTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "MultipleItemsAddedToArchetypeCorrectly", MultipleItemsAddedToArchetypeCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "RemovingLastKeepsFirst", RemovingLastKeepsFirst);
        testRegistry->AddTestCase("ArchetypeTest", "RemovingFirstKeepsLast", RemovingFirstKeepsLast);
    }

    static bool SinglePrimitiveTypeInitialisedCorrectly()
    {
        constexpr int initialValue = 2;

        ScarlEnt::Archetype<int> archetype;
        archetype.AddEntity(static_cast<int>(initialValue));

        return *archetype.GetComponent<int>(0) == initialValue;
    }

    static bool SingleCustomTypeInitialisedCorrectly()
    {
        constexpr int initialValue = 2;

        ScarlEnt::Archetype<Vec2> archetype;
        archetype.AddEntity({ initialValue, initialValue * initialValue });

        const Vec2* vector = archetype.GetComponent<Vec2>(0);

        return vector->x == initialValue && vector->y == initialValue * initialValue;
    }


    static bool DoublePrimitiveTypeInitialisedCorrectly()
    {
        constexpr int initialValue = 2;

        ScarlEnt::Archetype<int, long> archetype;
        archetype.AddEntity(static_cast<int>(initialValue), initialValue * initialValue);

        return *archetype.GetComponent<int>(0) == initialValue && *archetype.GetComponent<long>(0) == initialValue * initialValue;
    }

    static bool DoubleCustomTypeInitialisedCorrectly()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;

        ScarlEnt::Archetype<Vec2, Vec3> archetype;
        archetype.AddEntity(Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });

        const Vec2* vector2 = archetype.GetComponent<Vec2>(0);
        const Vec3* vector3 = archetype.GetComponent<Vec3>(0);

        return vector2->x == initialValue && vector2->y == initialSquare && 
               vector3->x == initialValue && vector3->y == initialSquare && vector3->z == initialCubed;
    }

    static bool MultipleItemsAddedToArchetypeCorrectly()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;

        ScarlEnt::Archetype<Vec2, Vec3> archetype;
        archetype.AddEntity(Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });
        archetype.AddEntity(Vec2{ initialCubed, initialSquare }, Vec3{ initialCubed, initialSquare, initialValue });

        const Vec2* vector2 = archetype.GetComponent<Vec2>(0);
        const Vec3* vector3 = archetype.GetComponent<Vec3>(0);

        const Vec2* other2 = archetype.GetComponent<Vec2>(1);
        const Vec3* other3 = archetype.GetComponent<Vec3>(1);

        return vector2->x == initialValue && vector2->y == initialSquare &&
               vector3->x == initialValue && vector3->y == initialSquare && vector3->z == initialCubed &&
               other2->x  == initialCubed && other2->y  == initialSquare &&
               other3->x  == initialCubed && other3->y  == initialSquare && other3->z == initialValue;
    }

    static bool RemovingLastKeepsFirst()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;

        ScarlEnt::Archetype<Vec2, Vec3> archetype;
        archetype.AddEntity(Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });
        archetype.AddEntity(Vec2{ initialCubed, initialSquare }, Vec3{ initialCubed, initialSquare, initialValue });

        archetype.RemoveEntity(1);

        const Vec2* vector2 = archetype.GetComponent<Vec2>(0);
        const Vec3* vector3 = archetype.GetComponent<Vec3>(0);

        return vector2->x == initialValue && vector2->y == initialSquare &&
               vector3->x == initialValue && vector3->y == initialSquare && vector3->z == initialCubed;
    }

    static bool RemovingFirstKeepsLast()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;

        ScarlEnt::Archetype<Vec2, Vec3> archetype;
        archetype.AddEntity(Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });
        archetype.AddEntity(Vec2{ initialCubed, initialSquare }, Vec3{ initialCubed, initialSquare, initialValue });

        archetype.RemoveEntity(0);

        const Vec2* vector2 = archetype.GetComponent<Vec2>(0);
        const Vec3* vector3 = archetype.GetComponent<Vec3>(0);

        return vector2->x == initialCubed && vector2->y == initialSquare &&
               vector3->x == initialCubed && vector3->y == initialSquare && vector3->z == initialValue;
    }
};

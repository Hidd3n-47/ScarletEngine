#pragma once

#include <ScarletTestTemplate/Core/TestRegistry.h>

#include <ScarlEnt/Archetype.h>

#include "SimpleTestTypes.h"

class ArchetypeTests
{
public:
    explicit inline ArchetypeTests(Scarlet::TestRegistry* testRegistry)
    {
#ifdef DEV_CONFIGURATION
        testRegistry->AddTestCase("ArchetypeTest", "ArchetypeAssertsIfTryingToAddEntityWithDifferentComponents", ArchetypeAssertsIfTryingToAddEntityWithDifferentComponents);
#endif // DEV_CONFIGURATION.
        testRegistry->AddTestCase("ArchetypeTest", "SinglePrimitiveTypeInitialisedCorrectly", SinglePrimitiveTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "SingleCustomTypeInitialisedCorrectly", SingleCustomTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "DoublePrimitiveTypeInitialisedCorrectly", DoublePrimitiveTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "DoubleCustomTypeInitialisedCorrectly", DoubleCustomTypeInitialisedCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "MultipleItemsAddedToArchetypeCorrectly", MultipleItemsAddedToArchetypeCorrectly);
        testRegistry->AddTestCase("ArchetypeTest", "RemovingLastKeepsFirst", RemovingLastKeepsFirst);
        testRegistry->AddTestCase("ArchetypeTest", "RemovingFirstKeepsLast", RemovingFirstKeepsLast);
        testRegistry->AddTestCase("ArchetypeTest", "GettingArrayFromArchetypeIsCorrect", GettingArrayFromArchetypeIsCorrect);
    }

    static bool ArchetypeAssertsIfTryingToAddEntityWithDifferentComponents()
    {
        bool passed = false;

        ScarlEnt::Archetype archetype(Scarlet::Ulid{}, Int{ 0 });

        try
        {
            archetype.AddEntity(Scarlet::Ulid{}, Char{ 'c' });
        }
        catch (std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool SinglePrimitiveTypeInitialisedCorrectly()
    {
        constexpr int initialValue = 2;
        const Scarlet::Ulid id;

        ScarlEnt::Archetype archetype(id, Int{ initialValue });

        return archetype.GetComponent<Int>(id) == Int{ initialValue };
    }

    static bool SingleCustomTypeInitialisedCorrectly()
    {
        constexpr int initialValue = 2;
        const Scarlet::Ulid id;

        ScarlEnt::Archetype archetype(id, Vec2{ initialValue, initialValue * initialValue });

        const Vec2& vector = archetype.GetComponent<Vec2>(id);

        return vector.x == initialValue && vector.y == initialValue * initialValue;
    }


    static bool DoublePrimitiveTypeInitialisedCorrectly()
    {
        constexpr int initialValue = 2;
        const Scarlet::Ulid id;

        ScarlEnt::Archetype archetype(id, Int{ initialValue }, Vec2{ 0, initialValue * initialValue });

        return archetype.GetComponent<Int>(id) == Int{ initialValue } && archetype.GetComponent<Vec2>(id) == Vec2{ 0, initialValue * initialValue };
    }

    static bool DoubleCustomTypeInitialisedCorrectly()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;
        const Scarlet::Ulid id;

        ScarlEnt::Archetype archetype(id, Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });

        const Vec2& vector2 = archetype.GetComponent<Vec2>(id);
        const Vec3& vector3 = archetype.GetComponent<Vec3>(id);

        return vector2.x == initialValue && vector2.y == initialSquare && 
               vector3.x == initialValue && vector3.y == initialSquare && vector3.z == initialCubed;
    }

    static bool MultipleItemsAddedToArchetypeCorrectly()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;
        const Scarlet::Ulid id1;
        const Scarlet::Ulid id2;

        ScarlEnt::Archetype archetype(id1, Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });
        archetype.AddEntity(id2, Vec2{ initialCubed, initialSquare }, Vec3{ initialCubed, initialSquare, initialValue });

        const Vec2& vector2 = archetype.GetComponent<Vec2>(id1);
        const Vec3& vector3 = archetype.GetComponent<Vec3>(id1);

        const Vec2& other2 = archetype.GetComponent<Vec2>(id2);
        const Vec3& other3 = archetype.GetComponent<Vec3>(id2);

        return vector2.x == initialValue && vector2.y == initialSquare &&
               vector3.x == initialValue && vector3.y == initialSquare && vector3.z == initialCubed &&
               other2.x  == initialCubed && other2.y  == initialSquare &&
               other3.x  == initialCubed && other3.y  == initialSquare && other3.z == initialValue;
    }

    static bool RemovingLastKeepsFirst()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;
        const Scarlet::Ulid id1;
        const Scarlet::Ulid id2;

        ScarlEnt::Archetype archetype(id1, Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });
        archetype.AddEntity(id2, Vec2{ initialCubed, initialSquare }, Vec3{ initialCubed, initialSquare, initialValue });

        archetype.RemoveEntity(id2);

        const Vec2& vector2 = archetype.GetComponent<Vec2>(id1);
        const Vec3& vector3 = archetype.GetComponent<Vec3>(id1);

        return vector2.x == initialValue && vector2.y == initialSquare &&
               vector3.x == initialValue && vector3.y == initialSquare && vector3.z == initialCubed;
    }

    static bool RemovingFirstKeepsLast()
    {
        constexpr int initialValue  = 2;
        constexpr int initialSquare = initialValue  * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;
        const Scarlet::Ulid id1;
        const Scarlet::Ulid id2;

        ScarlEnt::Archetype archetype(id1, Vec2{ initialValue, initialSquare }, Vec3{ initialValue, initialSquare, initialCubed });
        archetype.AddEntity(id2, Vec2{ initialCubed, initialSquare }, Vec3{ initialCubed, initialSquare, initialValue });

        archetype.RemoveEntity(id1);

        const Vec2& vector2 = archetype.GetComponent<Vec2>(id2);
        const Vec3& vector3 = archetype.GetComponent<Vec3>(id2);

        return vector2.x == initialCubed && vector2.y == initialSquare &&
               vector3.x == initialCubed && vector3.y == initialSquare && vector3.z == initialValue;
    }

    static bool GettingArrayFromArchetypeIsCorrect()
    {
        constexpr int initialValue  = 47;
        constexpr int initialSquare = initialValue * initialValue;
        constexpr int initialCubed  = initialSquare * initialValue;

        bool passed = true;

        ScarlEnt::Archetype archetype(Scarlet::Ulid{}, Int{ initialValue }, Vec2{}, Vec3{}, Char{ 'a' });
        archetype.AddEntity(Scarlet::Ulid{}, Int{ initialSquare }, Vec2{}, Vec3{}, Char{ 'b' });
        archetype.AddEntity(Scarlet::Ulid{}, Int{ initialCubed }, Vec2{}, Vec3{}, Char{ 'c' });

        const auto [intArray, charArray] = archetype.GetComponentArrays<Int, Char>();

        passed &= intArray->componentArray.size() == 3;
        passed &= intArray->componentArray[0] == Int{ initialValue };
        passed &= intArray->componentArray[1] == Int{ initialSquare};
        passed &= intArray->componentArray[2] == Int{ initialCubed };

        passed &= charArray->componentArray.size() == 3;
        passed &= charArray->componentArray[0] == Char{ 'a' };
        passed &= charArray->componentArray[1] == Char{ 'b' };
        passed &= charArray->componentArray[2] == Char{ 'c' };

        return passed;
    }
};

#pragma once

#include <stdexcept>

#include <ScarletTestTemplate/Core/TestRegistry.h>

#define SCARLENT_TEST
#include <ScarlEnt/SparseSet.h>

#include "SimpleTestTypes.h"

class SparseSetTests
{
public:
    static constexpr size_t page_size = 1000;
    static constexpr int    number    = 47;

    explicit inline SparseSetTests(Scarlet::TestRegistry* testRegistry)
    {
        testRegistry->AddTestCase("SparseSetTest", "SinglePrimitiveTypeInitialisedCorrectly", SinglePrimitiveTypeInitialisedCorrectly);
        testRegistry->AddTestCase("SparseSetTest", "SingleCustomTypeInitialisedCorrectly", SingleCustomTypeInitialisedCorrectly);
        testRegistry->AddTestCase("SparseSetTest", "DoublePrimitiveTypeInitialisedCorrectly", DoublePrimitiveTypeInitialisedCorrectly);
        testRegistry->AddTestCase("SparseSetTest", "DoubleCustomTypeInitialisedCorrectly", DoubleCustomTypeInitialisedCorrectly);
        testRegistry->AddTestCase("SparseSetTest", "DoubleCustomTypeInitialisedWithDifferentPagesCorrectly", DoubleCustomTypeInitialisedWithDifferentPagesCorrectly);
        testRegistry->AddTestCase("SparseSetTest", "AddingTwoOfSameIdAsserts", AddingTwoOfSameIdAsserts);
        testRegistry->AddTestCase("SparseSetTest", "RemovingIdThatIsNotPresentAsserts", RemovingIdThatIsNotPresentAsserts);
        testRegistry->AddTestCase("SparseSetTest", "RemovingSameIdAsserts", RemovingSameIdAsserts);
        testRegistry->AddTestCase("SparseSetTest", "PopRetrievesAndRemovesItem", PopRetrievesAndRemovesItem);
        testRegistry->AddTestCase("SparseSetTest", "PopRetrievesAndRemovesItemWhenGettingLastItem", PopRetrievesAndRemovesItemWhenGettingLastItem);
        testRegistry->AddTestCase("SparseSetTest", "PopRetrievesAndRemovesItemWhenGettingFirstItem", PopRetrievesAndRemovesItemWhenGettingFirstItem);
        testRegistry->AddTestCase("SparseSetTest", "PopRetrievesAndRemovesItemWhenGettingMiddleItem", PopRetrievesAndRemovesItemWhenGettingMiddleItem);
        testRegistry->AddTestCase("SparseSetTest", "SquareBracketsRetrieveCorrectItemWhenOneItem", SquareBracketsRetrieveCorrectItemWhenOneItem);
        testRegistry->AddTestCase("SparseSetTest", "SquareBracketsRetrieveCorrectItemWhenTwoItems", SquareBracketsRetrieveCorrectItemWhenTwoItems);
        testRegistry->AddTestCase("SparseSetTest", "SquareBracketsRetrieveCorrectItemWhenThreeItems", SquareBracketsRetrieveCorrectItemWhenThreeItems);
        testRegistry->AddTestCase("SparseSetTest", "SquareBracketsReturnReferenceToUpdateOriginal", SquareBracketsReturnReferenceToUpdateOriginal);
        testRegistry->AddTestCase("SparseSetTest", "ContainsReturnTrueWhenItemInSparseSet", ContainsReturnTrueWhenItemInSparseSet);
        testRegistry->AddTestCase("SparseSetTest", "ContainsReturnFalseWhenItemInSparseSet", ContainsReturnFalseWhenItemInSparseSet);
        testRegistry->AddTestCase("SparseSetTest", "ContainsReturnFalseWhenItemRemoved", ContainsReturnFalseWhenItemRemoved);
    }

    static bool SinglePrimitiveTypeInitialisedCorrectly()
    {
        bool passed = true;

        static constexpr uint64 id = 0;
        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);

        const size_t pageIndex = ss.GetPageIndexForId(id);
        const size_t indexPage = ss.GetIndexInPageForId(id);

        passed &= ss.GetDense().size()      == 1;      // Should only be one item in dense list.
        passed &= ss.GetDense()[0]          == number; // Dense list should be value passed in.
        passed &= ss.GetDenseToId().size()  == 1;      // Dense list to ID should only have one value.
        passed &= ss.GetDenseToId()[0]      == id;     // Dense list to ID should be the ID added.
        passed &= ss.GetSparse().capacity() >= 1;      // Greater than as it could be overallocated - should only have a single page.
        passed &= (*ss.GetSparse()[pageIndex])[indexPage]  == 0 + ss.id_offset; // The id is mapped to page 0, and ID of id, it should be equal to index 0 plus the offset.

        return passed;
    }

    static bool SingleCustomTypeInitialisedCorrectly()
    {
        bool passed = true;

        static constexpr uint64 id = 475;
        constexpr Vec2 solution{ number, number * number };

        ScarlEnt::SparseSet<Vec2, page_size> ss;
        ss.Add(id, number, number * number);

        const size_t pageIndex = ss.GetPageIndexForId(id);
        const size_t indexPage = ss.GetIndexInPageForId(id);

        passed &= ss.GetDense().size()      == 1;        // Should only be one item in dense list.
        passed &= ss.GetDense()[0]          == solution; // Dense list should be value passed in.
        passed &= ss.GetDenseToId().size()  == 1;        // Dense list to ID should only have one value.
        passed &= ss.GetDenseToId()[0]      == id;       // Dense list to ID should be the ID added.
        passed &= ss.GetSparse().capacity() >= 1;        // Greater than as it could be overallocated - should only have a single page.
        passed &= (*ss.GetSparse()[pageIndex])[indexPage]  == 0 + ss.id_offset; // The id is mapped to page 0, and ID of id, it should be equal to index 0 plus the offset.

        return passed;
    }

    static bool DoublePrimitiveTypeInitialisedCorrectly()
    {
        bool passed = true;

        static constexpr uint64 id = 238;
        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);
        ss.Add(id + id, number * number);

        const size_t pageIndex1 = ss.GetPageIndexForId(id);
        const size_t pageIndex2 = ss.GetPageIndexForId(id + id);
        const size_t indexPage1 = ss.GetIndexInPageForId(id);
        const size_t indexPage2 = ss.GetIndexInPageForId(id + id);

        passed &= ss.GetDense().size()      == 2;               // Should only be one item in dense list.
        passed &= ss.GetDense()[0]          == number;          // First dense list item should be value passed in.
        passed &= ss.GetDense()[1]          == number * number; // Second dense list item should be value passed in.
        passed &= ss.GetDenseToId().size()  == 2;               // Dense list to ID should only have one value.
        passed &= ss.GetDenseToId()[0]      == id;              // Dense list to ID should be the ID added.
        passed &= ss.GetDenseToId()[1]      == id + id;         // Dense list to ID should be the ID added.
        passed &= ss.GetSparse().capacity() >= 1;               // Greater than as it could be overallocated - should only have a single page.
        passed &= (*ss.GetSparse()[pageIndex1])[indexPage1]  == 0 + ss.id_offset; // The id is mapped to page 0, and ID of id, it should be equal to index 0 plus the offset.
        passed &= (*ss.GetSparse()[pageIndex2])[indexPage2]  == 1 + ss.id_offset; // The id is mapped to page 0, and ID of id + id, it should be equal to index 1 plus the offset.

        return passed;
    }

    static bool DoubleCustomTypeInitialisedCorrectly()
    {
        bool passed = true;

        static constexpr uint64 id = 475;
        constexpr Vec2 solution1{ number, number * number };
        constexpr Vec2 solution2{ number * number, number / number };

        ScarlEnt::SparseSet<Vec2, page_size> ss;
        ss.Add(id, number, number * number);
        ss.Add(id + id, number * number, number / number);

        const size_t pageIndex1 = ss.GetPageIndexForId(id);
        const size_t pageIndex2 = ss.GetPageIndexForId(id + id);
        const size_t indexPage1 = ss.GetIndexInPageForId(id);
        const size_t indexPage2 = ss.GetIndexInPageForId(id + id);

        passed &= ss.GetDense().size()      == 2;         // Should only be one item in dense list.
        passed &= ss.GetDense()[0]          == solution1; // First dense list item should be value passed in.
        passed &= ss.GetDense()[1]          == solution2; // Second dense list item should be value passed in.
        passed &= ss.GetDenseToId().size()  == 2;         // Dense list to ID should only have one value.
        passed &= ss.GetDenseToId()[0]      == id;        // Dense list to ID should be the ID added.
        passed &= ss.GetDenseToId()[1]      == id + id;   // Dense list to ID should be the ID added.
        passed &= ss.GetSparse().capacity() >= 1;         // Greater than as it could be overallocated - should only have a single page.
        passed &= (*ss.GetSparse()[pageIndex1])[indexPage1] == 0 + ss.id_offset; // The id is mapped to page 0, and ID of id, it should be equal to index 0 plus the offset.
        passed &= (*ss.GetSparse()[pageIndex2])[indexPage2] == 1 + ss.id_offset; // The id is mapped to page 0, and ID of id + id, it should be equal to index 1 plus the offset.

        return passed;
    }

    static bool DoubleCustomTypeInitialisedWithDifferentPagesCorrectly()
    {
        bool passed = true;

        static constexpr uint64 id = page_size / 2 + 20;
        constexpr Vec2 solution1{ number, number * number };
        constexpr Vec2 solution2{ number * number, number / number };

        ScarlEnt::SparseSet<Vec2, page_size> ss;
        ss.Add(id, number, number * number);
        ss.Add(id + id, number * number, number / number);

        const size_t pageIndex1 = ss.GetPageIndexForId(id);
        const size_t pageIndex2 = ss.GetPageIndexForId(id + id);
        const size_t indexPage1 = ss.GetIndexInPageForId(id);
        const size_t indexPage2 = ss.GetIndexInPageForId(id + id);

        passed &= ss.GetDense().size()      == 2;         // Should only be one item in dense list.
        passed &= ss.GetDense()[0]          == solution1; // First dense list item should be value passed in.
        passed &= ss.GetDense()[1]          == solution2; // Second dense list item should be value passed in.
        passed &= ss.GetDenseToId().size()  == 2;         // Dense list to ID should only have one value.
        passed &= ss.GetDenseToId()[0]      == id;        // Dense list to ID should be the ID added.
        passed &= ss.GetDenseToId()[1]      == id + id;   // Dense list to ID should be the ID added.
        passed &= ss.GetSparse().capacity() >= 2;         // Greater than as it could be overallocated - should have at least 2 pages.
        passed &= (*ss.GetSparse()[pageIndex1])[indexPage1] == 0 + ss.id_offset; // The id is mapped to page 0, and ID of id, it should be equal to index 0 plus the offset.
        passed &= (*ss.GetSparse()[pageIndex2])[indexPage2] == 1 + ss.id_offset; // The id is mapped to page 0, and ID of id + id, it should be equal to index 1 plus the offset.

        return passed;
    }

    static bool AddingTwoOfSameIdAsserts()
    {
        bool passed = false;

        static constexpr uint64 id = 238;
        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);

        try
        {
            ss.Add(id, number);
        } 
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool RemovingIdThatIsNotPresentAsserts()
    {
        bool passed = false;

        static constexpr uint64 id = 238;
        ScarlEnt::SparseSet<int, page_size> ss;

        try
        {
            ss.Remove(id);
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool RemovingSameIdAsserts()
    {
        bool passed = false;

        static constexpr uint64 id = 238;
        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);
        ss.Remove(id);

        try
        {
            ss.Remove(id);
        }
        catch (const std::runtime_error&)
        {
            passed = true;
        }

        return passed;
    }

    static bool PopRetrievesAndRemovesItem()
    {
        bool passed = true;

        static constexpr uint64 id = 238;
        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);

        const int popped = ss.Pop(id);

        passed &= popped == number;

        try
        {
            ss.Remove(id);
        }
        catch (const std::runtime_error&)
        {
            passed &= true;
        }

        return passed;
    }

    static bool PopRetrievesAndRemovesItemWhenGettingLastItem()
    {
        bool passed = true;

        static constexpr uint64 id1 = 238;
        static constexpr uint64 id2 = 567;

        constexpr int solution = number * number;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id1, number);
        ss.Add(id2, solution);

        const int popped = ss.Pop(id2);

        passed &= popped == solution;

        try
        {
            ss.Remove(id2);
        }
        catch (const std::runtime_error&)
        {
            passed &= true;
        }

        ss.Remove(id1);

        return passed;
    }

    static bool PopRetrievesAndRemovesItemWhenGettingFirstItem()
    {
        bool passed = true;

        static constexpr uint64 id1 = 238;
        static constexpr uint64 id2 = 567;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id1, number);
        ss.Add(id2, number * number);

        const int popped = ss.Pop(id1);

        passed &= popped == number;

        try
        {
            ss.Remove(id1);
        }
        catch (const std::runtime_error&)
        {
            passed &= true;
        }

        ss.Remove(id2);

        return passed;
    }

    static bool PopRetrievesAndRemovesItemWhenGettingMiddleItem()
    {
        bool passed = true;

        static constexpr uint64 id1 = 129;
        static constexpr uint64 id2 = 238;
        static constexpr uint64 id3 = 567;

        constexpr int solution = number * number;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id1, number);
        ss.Add(id2, solution);
        ss.Add(id3, solution * number);

        const int popped = ss.Pop(id2);

        passed &= popped == solution;

        try
        {
            ss.Remove(id2);
        }
        catch (const std::runtime_error&)
        {
            passed &= true;
        }

        ss.Remove(id1);
        ss.Remove(id3);

        return passed;
    }

    static bool SquareBracketsRetrieveCorrectItemWhenOneItem()
    {
        bool passed = true;

        static constexpr uint64 id1 = 129;

        constexpr int solution = number * number;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id1, solution);

        const int returnedNumber = ss[id1];

        passed &= returnedNumber == solution;

        return passed;
    }

    static bool SquareBracketsRetrieveCorrectItemWhenTwoItems()
    {
        bool passed = true;

        static constexpr uint64 id1 = 129;
        static constexpr uint64 id2 = 238;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id1, number);
        ss.Add(id2, number * number);

        const int returnedNumber1 = ss[id1];
        const int returnedNumber2 = ss[id2];

        passed &= returnedNumber1 == number;
        passed &= returnedNumber2 == number * number;

        return passed;
    }

    static bool SquareBracketsRetrieveCorrectItemWhenThreeItems()
    {
        bool passed = true;

        static constexpr uint64 id1 = 129;
        static constexpr uint64 id2 = 238;
        static constexpr uint64 id3 = 567;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id1, number);
        ss.Add(id2, number * number);
        ss.Add(id3, number * number * number);

        const int returnedNumber1 = ss[id1];
        const int returnedNumber2 = ss[id2];
        const int returnedNumber3 = ss[id3];

        passed &= returnedNumber1 == number;
        passed &= returnedNumber2 == number * number;
        passed &= returnedNumber3 == number * number * number;

        return passed;
    }

    static bool SquareBracketsReturnReferenceToUpdateOriginal()
    {

        bool passed = true;

        static constexpr uint64 id = 129;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);

        ss[id] = id;

        passed &= ss[id] == id;

        return passed;
    }

    static bool ContainsReturnTrueWhenItemInSparseSet()
    {
        bool passed = true;

        static constexpr uint64 id = 129;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);

        passed &= ss.Contains(id);

        return passed;
    }

    static bool ContainsReturnFalseWhenItemInSparseSet()
    {
        bool passed = true;

        static constexpr uint64 id = 129;

        ScarlEnt::SparseSet<int, page_size> ss;

        passed &= !ss.Contains(id);

        return passed;
    }

    static bool ContainsReturnFalseWhenItemRemoved()
    {
        bool passed = true;

        static constexpr uint64 id = 129;

        ScarlEnt::SparseSet<int, page_size> ss;
        ss.Add(id, number);

        passed &= ss.Contains(id);

        ss.Remove(id);

        passed &= !ss.Contains(id);

        return passed;
    }
};

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
};

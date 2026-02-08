#pragma once

#include <ScarletCore/array.h>
#include <ScarletCore/vector.h>

#include <ScarletCore/PrimitiveTypes.h>

#include "Defines.h"

namespace ScarlEnt
{
/**
 * @class SparseSet: A sparse set is a data structure to ensure contiguous storage \\n
 * of the data, whilst being able to add and remove. The sparse-array allows for a mapping \\n
 * between an identifier to the data. Allowing for constant time operations to add, remove \\n
 * and retrieve elements. \nn
 * The sparse-set makes use of pagination to reduce memory overhead in the sparse array. \\n
 * @tparam T The type the dense array stores within the \ref SparseSet.
 * @tparam PageSize The number of items stored in each page. \\n
 * The smaller the page size the more densely packed the sparse array will be, however, it will be closer to a map, causing more overhead due to more iteration.
 */
template <typename T, size_t PageSize>
class SparseSet
{
public:
    using PagedSparse = vector<array<size_t, PageSize>*>;

    /**
     * @brief Add an item with ID to the \ref SparseSet.
     * @tparam Args The argument types used to construct \c T.
     * @param id The unique ID of the item being added to the \ref SparseSet.
     * @param args The arguments forwarded to construct type \c T.
     */
    template <typename...Args>
    inline void Add(const uint64 id, Args&&...args)
    {
        // The sparse-array is a vector of array pointers.
        // It makes use of pointers to prevent allocation of the arrays whilst allowing for ordered pages.
        // Pages are only allocated when finally indexed in.

        const size_t pageIndex = GetPageIndexForId(id);

        // Ensure that the ID is not already present in the sparse set.
        SCARLENT_ASSERT(mSparse.capacity() <= pageIndex                    // <- Capacity is less than page index, therefore not added.
                     || mSparse[pageIndex] == nullptr                      // <- Page doesn't exist, therefore not added.
                     || (*mSparse[pageIndex])[id % PageSize] == invalid_id // <- ID is invalid_id, therefore not added.
                     && "Trying to add ID that is already present.");

        // Add data to the dense array, and keep track of the most recent ID that has been allocated in a dense array.
        // This dense array of ID is used when removing when the back is swapped.
        mDense.emplace_back(std::forward<Args>(args)...);
        mDenseToId.emplace_back(id);

        if (mSparse.capacity() <= pageIndex)
        {
            // Resize the capacity to be double (add one for the case that initial size is 0).
            // This will overallocate, however only overallocating the size of pointers.
            mSparse.resize(pageIndex * 2 + 1);
        }

        if (mSparse[pageIndex] == nullptr)
        {
            mSparse[pageIndex] = new array<size_t, PageSize>();
            // Todo Christian: Do some testing to see if this fill will be a compile time optimised memset, or if this adds runtime overhead.
            // mSparse[pageIndex]->fill(-1);
        }

        // We remove one to get to 0 index based and add the id_offset.
        // Note this is redundant as -1+1=0, but added for readability.
        // All IDs are offset by id_offset to map 0 to invalid_id.
        (*mSparse[pageIndex])[GetIndexInPageForId(id)] = mDense.size() - 1 + id_offset;
    }

    /**
     * @brief Remove the item with corresponding ID from the dense array.
     * @param id The \c id being removed.
     */
    inline void Remove(const uint64 id)
    {
        const size_t pageIndex = GetPageIndexForId(id);

        // Ensure that the ID is present in the sparse set.
        SCARLENT_ASSERT(mSparse.capacity() >  pageIndex                    // <- Capacity is greater than page index, therefore possibly added.
                     && mSparse[pageIndex] != nullptr                      // <- Page exists, therefore possibly added.
                     && (*mSparse[pageIndex])[id % PageSize] != invalid_id // <- ID is not invalid_id, therefore ID is mapped to some part in the dense array.
                     && "Trying to remove an ID that is not present in the sparse-set.");

        // IDs are offset (not 0 index based so that 0 is invalid ID), remove id_offset to get true id.
        const size_t index = (*mSparse[pageIndex])[id % PageSize] - id_offset;
        const size_t last  = mDenseToId.back();

        // If the last element is not being removed, swap elements and update indices.
        if (index != mDense.size() - 1)
        {
            // Swap back and then pop back.
            std::swap(mDense[index], mDense.back());
            std::swap(mDenseToId[index], mDenseToId.back());

            // Update the moved ID to be the index + id_offset as 0 is mapped to invalid_id.
            (*mSparse[last / PageSize])[last % PageSize] = index + id_offset;
        }

        // Invalidate the removed ID.
        (*mSparse[pageIndex])[GetIndexInPageForId(id)] = invalid_id;

        mDense.pop_back();
        mDenseToId.pop_back();
    }

    /**
     * @brief Get the index of the page that a given ID exists in.
     * @param id The \c id whose page index is being calculated.
     * @return The index of the page that the given ID exists in.
     */
    [[nodiscard]] inline static size_t GetPageIndexForId(const uint64_t id) { return id / PageSize; }
    /**
     * @brief Get the index inside the page for a given ID.
     * @param id The \c id whose index in a given page is being calculated.
     * @return Get the index inside the page for a given ID.
     */
    [[nodiscard]] inline static size_t GetIndexInPageForId(const uint64_t id) { return id % PageSize; }

    // To prevent having to memset arrays, 0 is defaulted to be the invalid ID.
    static constexpr uint64 invalid_id = 0;
    static constexpr uint64 id_offset  = 1;

#ifdef SCARLENT_TEST
    /** @brief Get a constant reference to the dense array.       */
    [[nodiscard]] inline const vector<T>&      GetDense()     const { return mDense; }
    /** @brief Get a constant reference to the dense to ID array. */
    [[nodiscard]] inline const vector<uint64>& GetDenseToId() const { return mDenseToId; }
    /** @brief Get a constant reference to the parse array.       */
    [[nodiscard]] inline const PagedSparse&    GetSparse()    const { return mSparse; }
#endif // SCARLENT_TEST.

private:
    vector<T>      mDense;
    PagedSparse    mSparse;
    vector<uint64> mDenseToId;

};

} // Namespace ScarlEnt

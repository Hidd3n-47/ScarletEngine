#pragma once

namespace Scarlet
{

/**
 * @class WeakHandle: A class used to hold a reference to a pointer such that the ownership is not transferred.\\n
 * Having a handle to a pointer allows you to access the pointer and its methods but not call \c delete on it.
 * @note A handle could be invalid if the original memory is freed, and therefore should be only used when the lifetime is known to exist.
 * @tparam T: The class of the pointer.
 */
template <typename T>
class WeakHandle
{
public:
    WeakHandle()  = default;
    ~WeakHandle() = default;

    WeakHandle(const WeakHandle&)            = default;
    WeakHandle(WeakHandle&&)                 = default;
    WeakHandle& operator=(WeakHandle&&)      = default;
    WeakHandle& operator=(const WeakHandle&) = default;

    inline explicit WeakHandle(T* reference) noexcept
        : mReferencePtr(reference)
    { /* Empty. */ }

    /**
     * A method used to check if the pointer is still valid.
     * @return \c true if the reference is still valid, \c false otherwise.
     */
    inline bool IsValid() const { return mReferencePtr != nullptr; }
    /**
     * A method used to invalidate a pointer.
     */
    inline void Invalidate() noexcept { mReferencePtr = nullptr; }

    /**
     * @brief Cast a \c WeakHandle to be a \c WeakHandle of a different class. Useful when trying to cast for inheritance.
     * @tparam CastType The class that the reference is being cast to.
     * @note This \c WeakHandle could be invalid if the cast failed, and hence should be checked if it is valid before use.
     * @return A \c WeakHandle that has been cast to the type \c CastType. 
     */
    template <typename CastType>
    [[nodiscard]] inline WeakHandle<CastType> Cast() { return WeakHandle<CastType>{ dynamic_cast<CastType*>(mReferencePtr) }; }

    /**
     * @brief Get the raw pointer that the handle contains.
     * @note This should not be used other than by internal systems that need access to the raw pointer as it defeats the purpose of the weak handle.
     * @return The original pointer.
     */
    [[nodiscard]] T* GetRawPtr() const { return mReferencePtr; }

    inline T* operator->()             { return mReferencePtr; }
    inline const T* operator->() const { return mReferencePtr; }

    [[nodiscard]] inline bool operator==(const WeakHandle<T> other) { return mReferencePtr == other.mReferencePtr; }
    [[nodiscard]] inline bool operator==(const WeakHandle<T> other) const { return mReferencePtr == other.mReferencePtr; }
private:
    T* mReferencePtr = nullptr;
};

} // Namespace Scarlet.

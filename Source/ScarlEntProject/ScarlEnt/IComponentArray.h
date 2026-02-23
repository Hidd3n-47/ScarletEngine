#pragma once

namespace ScarlEnt
{
/**
 * @struct IComponentArray: An interface for the component arrays. This is used to erase type information to allow for dynamic storage of multiple component arrays.
 */
struct IComponentArray
{
    virtual ~IComponentArray() = default;

    /**
     * @brief Remove a component at a given index from the array.
     * @param index The index of the given component being removed.
     */
    virtual void RemoveComponent(const size_t index) = 0;

};

/**
 * @struct ComponentArray: A wrapper to hold the component array. This struct holds the type information that \ref IComponentArray strips out.
 * @tparam T The type of the component.
 */
template <typename T>
struct ComponentArray : IComponentArray
{
    /**
     * @brief Add a component to the array.
     * @param data The component data cast to a void pointer to remove type information.
     */
    inline void AddComponent(T&& data)
    {
        componentArray.emplace_back(std::move(data));
    }

    /**
     * @copydoc IComponentArray::RemoveComponent
     */
    inline void RemoveComponent(const size_t index) override
    {
        componentArray[index] = componentArray.back();
        componentArray.pop_back();
    }

    /**
     * @brief Get a component at the given index from the array.
     * @param index The index of the given component being requested.
     * @return A reference to the component data.
     */
    [[nodiscard]] inline T& GetComponent(const size_t index)
    {
        return static_cast<void*>(&componentArray[index]);
    }

    vector<T> componentArray;
};

} // Namespace ScarlEnt.
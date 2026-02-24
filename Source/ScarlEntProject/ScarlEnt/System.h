#pragma once

#include <functional>

#include <ScarletCore/PrimitiveTypes.h>

#include "Archetype.h"

namespace ScarlEnt
{

class Scene;
class Archetype;

/**
 * @struct ISystem: An interface for systems, this interface is used to strip the type information out of the system allowing for dynamic storage of systems.\\n
 * A system is something that will be used to manipulate component data in bulk.
 */
struct ISystem
{
    virtual ~ISystem() = default;

    /**
     * @brief Update the system by calling the systems update function on a specific archetype.
     * @param archetype The archetype of components that the systems update function will act on.
     */
    virtual void Update(Archetype* archetype) = 0;

    uint64 componentBitset;
};

/**
 * @class System: A structure representing a system (\ref ISystem) with its type information. This type information can be used to get all the components of the type \\n
 * from the archetype and update.
 * @tparam Components The components that the system requires as a part of its update.
 */
template <typename...Components>
struct System : ISystem
{
    explicit System(const std::function<void(Components&...)>& updateFunction) noexcept
        : updateFunction(updateFunction) 
    {
        componentBitset = (Registry::Instance().GetOrRegisterComponentBit<Components>() | ...);
    }

    /**
     * @copydoc ISystem::Update
     */
    void Update(Archetype* archetype) override
    {
        auto components = archetype->GetComponentArrays<Components...>();

        std::apply(
            [&](auto&... arrays)
            {
                const std::size_t size = archetype->GetSize();

                for (std::size_t i = 0; i < size; ++i)
                {
                    updateFunction(arrays->componentArray[i]...);
                }
            },
            components
        );
    }

    std::function<void(Components&...)> updateFunction;
};

} // Namespace ScarlEnt.
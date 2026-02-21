#include "ComponentManager.h"

#include <ranges>

namespace ScarlEnt
{

ComponentManager::~ComponentManager()
{
    for (const auto& array : mComponents | std::views::values)
    {
        delete array;
    }
}

} // Namespace ScarlEnt.
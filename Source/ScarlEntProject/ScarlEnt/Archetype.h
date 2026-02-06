#pragma once

#include <tuple>

#include "Defines.h"

template<typename...Components>
struct SCARLENT_API Archetype
{
    template <typename Component>
    inline Component* GetComponent()
    {
        return &std::get<Component>(components);
    }

    std::tuple<Components...> components;
};

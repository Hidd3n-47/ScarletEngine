#pragma once

#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include "Components/Transform.h"

template <typename T>
static void RegisterComponentTypeAndFunctionPointer(ScarlEnt::Registry& registry)
{
    const std::string componentName = registry.GetOrRegisterComponentId<T>().name;
    (void)registry.RegisterComponent(componentName, [](ScarlEnt::IEntityHandle* handle) {
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            mutableHandle->AddComponent<Scarlet::Component::Transform>();
        });
}

inline void RegisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Transform>(registry);
}

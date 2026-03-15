#pragma once

#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include "Components/Camera.h"
#include "Components/DirectionLight.h"
#include "Components/Mesh.h"
#include "Components/Transform.h"

template <typename T>
static void RegisterComponentTypeAndFunctionPointer(ScarlEnt::Registry& registry)
{
    const std::string componentName = registry.GetOrRegisterComponentId<T>().name;
    registry.RegisterComponent(componentName, [](ScarlEnt::IEntityHandle* handle) {
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            mutableHandle->AddComponent<T>();
        });
}

inline void RegisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Camera>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::DirectionLight>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Mesh>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Transform>(registry);
}

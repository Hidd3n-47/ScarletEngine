#pragma once

#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include "ScarletCoreEcs/Components/Camera.h"
#include "ScarletCoreEcs/Components/DirectionLight.h"
#include "ScarletCoreEcs/Components/Mesh.h"
#include "ScarletCoreEcs/Components/Transform.h"

template <typename T>
static void RegisterComponentTypeAndFunctionPointer(ScarlEnt::Registry& registry)
{
    const std::string componentName = registry.GetOrRegisterComponentId<T>().name;
    registry.RegisterComponent(componentName, [](ScarlEnt::IEntityHandle* handle) {
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            mutableHandle->AddComponent<T>();
            return mutableHandle->GetComponent<T>().GetProperties();
        });
}

struct COMPONENT_API Register
{
    static void RegisterComponents();
};

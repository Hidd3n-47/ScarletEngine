#pragma once

#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include "ScarletCoreEcs/Components/BoundingBox.h"
#include "ScarletCoreEcs/Components/Camera.h"
#include "ScarletCoreEcs/Components/DirectionLight.h"
#include "ScarletCoreEcs/Components/EditorInfo.h"
#include "ScarletCoreEcs/Components/SphereCollider.h"
#include "ScarletCoreEcs/Components/StaticMesh.h"
#include "ScarletCoreEcs/Components/Transform.h"


#ifdef DEV_CONFIGURATION

template <typename T>
static void RegisterComponentTypeAndFunctionPointer(ScarlEnt::Registry& registry)
{
    const std::string componentName = registry.GetOrRegisterComponentId<T>().name;
    registry.RegisterComponent(componentName, {
        .addComponent = [](ScarlEnt::IEntityHandle* handle) {
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            return mutableHandle->AddComponent<T>().GetProperties();
        },
        .hasComponent = [](ScarlEnt::IEntityHandle* handle) {
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            return mutableHandle->HasComponent<T>();
        },
        .removeComponent = [](ScarlEnt::IEntityHandle* handle) {
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            return mutableHandle->RemoveComponent<T>();
        }
    });
}

template <typename T>
static void UnregisterComponentType(ScarlEnt::Registry& registry)
{
    const std::string componentName = registry.GetOrRegisterComponentId<T>().name;
    registry.UnregisterComponentType(componentName);
}


struct Register
{
    static void RegisterComponents();
    static void UnregisterComponents();
};

#endif // DEV_CONFIGURATION.


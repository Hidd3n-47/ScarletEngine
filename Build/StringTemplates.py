
GENERATE_PROPERTIES_TEMPLATE = '''\
#include "Components/{struct_name}.h"

#include <ScarlEnt/ComponentManager.h>

#include <ScarletReflect/ReflectType.h>

namespace Scarlet::Component
{{

#ifdef DEV_CONFIGURATION

void {struct_name}::GenerateProperties()
{{
    mProperties.clear();
'''

REGISTER_COMPONENT_HEADER_TEMPLATE = '''\
#pragma once

#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

{component_includes}

#ifdef DEV_CONFIGURATION

template <typename T>
static void RegisterComponentTypeAndFunctionPointer(ScarlEnt::Registry& registry)
{{
    const std::string componentName = registry.GetOrRegisterComponentId<T>().name;
    registry.RegisterComponent(componentName, {{
        .addComponent = [](ScarlEnt::IEntityHandle* handle) {{
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            return mutableHandle->AddComponent<T>().GetProperties();
        }},
        .hasComponent = [](ScarlEnt::IEntityHandle* handle) {{
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            return mutableHandle->HasComponent<T>();
        }},
        .removeComponent = [](ScarlEnt::IEntityHandle* handle) {{
            ScarlEnt::MutableEntityHandle* mutableHandle = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);
            return mutableHandle->RemoveComponent<T>();
        }}
    }});
}}

struct Register
{{
    static void RegisterComponents();
}};

#endif // DEV_CONFIGURATION.

'''

REGISTER_COMPONENT_SOURCE_TEMPLATE = '''\
#include "Register.generated.h"

#ifdef DEV_CONFIGURATION

void Register::RegisterComponents()
{{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

{components_registry}
}}

#endif // DEV_CONFIGURATION.
'''

GENERATE_PROPERTY_TEMPLATE = '''\
    mProperties["{property_name}"] = ScarlEnt::Property {{
        ScarlEnt::PropertyType::{type_enum},
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<{struct_name}>(),
        [this] {{ return ReflectType::GetStringFromValue(this->{property_name}); }},
        [this](const std::string_view& stringValue) {{ ReflectType::SetValueFromString(this->{property_name}, stringValue); }}
    }};
'''

END_TEMPLATE = '''\
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.
'''
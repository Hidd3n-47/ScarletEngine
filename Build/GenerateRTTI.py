import os
import re
from typing import Optional, Match

ROOT_SEARCH_DIR             = "E:/Programming/ScarletEngine/Source/ScarletEngineProject/ScarletEngine/Components/"
GENERATED_RTTI_OUTPUT_DIR   = ROOT_SEARCH_DIR + "Generated/"

COMPONENT_MACRO = "SCARLET_COMPONENT("

types = {
     "uint32"       : "UINT32"  ,
     "float"        : "FLOAT"   ,
     "std::string"  : "STRING"  ,
     "Math::Vec3"   : "VEC3"    ,
     "Math::Vec4"   : "VEC4"    ,
     "Math::Quat"   : "QUAT"    ,
     "WeakHandle<Resource::ILazyLoadAsset>" : "ASSET",
}

components = []

# Function used to find all files that are considered "Components"
# These are files that have the C++ macro 'SCARLET_COMPONENT()'
def get_component_files(path):
    result = []
    for root, dirs, files in os.walk(path):
        for file in files:
            filepath = os.path.join(root,file)

            if not str(filepath).endswith(".h"):
                continue

            component_found = False
            with open(filepath, "r") as f:
                content = f.read()
                component_lines = content.splitlines()
                for line in component_lines:
                    if line.find(COMPONENT_MACRO) != -1:
                        component_found = True

            if component_found:
                result.append(component_lines)
    return result

def generate_rtti_for_component(struct_name, member_variables):
    os.makedirs(GENERATED_RTTI_OUTPUT_DIR, exist_ok=True)
    output_path = os.path.join(GENERATED_RTTI_OUTPUT_DIR, struct_name + ".generated.cpp")

    property_string = ""
    for variable in member_variables:
        property_string += "\n    { \"" + variable[1] + "\", Property{} },"

    with open(output_path, "w+") as f:
        f.write('''\
#include "ScarletEnginePch.h"
#include "Components/{struct_name}.h"

#include <ScarlEnt/ComponentManager.h>
#include "RTTI/ReflectType.h"

namespace Scarlet::Component
{{

#ifdef DEV_CONFIGURATION

void {struct_name}::GenerateProperties()
{{
    mProperties.clear();
'''.format(struct_name=struct_name))

        for variable in member_variables:
            if variable[0] in types:
                f.write('''\

    mProperties["{property_name}"] = ScarlEnt::Property {{
        ScarlEnt::PropertyType::{type_enum},
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<{struct_name}>(),
        [this] {{ return ReflectType::GetStringFromValue(this->{property_name}); }},
        [this](const std::string_view& stringValue) {{ ReflectType::SetValueFromString(this->{property_name}, stringValue); }}
    }};
'''.format(struct_name=struct_name, type=variable[0], type_enum=types[variable[0]], property_name=variable[1]))
            else:
                print("Error processing variable type: " + variable[0])

        f.write('''\
}};

#endif // DEV_CONFIGURATION.

}} // Namespace Scarlet::Component.
'''.format(struct_name=struct_name))

def register_components():
    os.makedirs(GENERATED_RTTI_OUTPUT_DIR, exist_ok=True)
    output_path = os.path.join(GENERATED_RTTI_OUTPUT_DIR, "Register.generated.h")

    with open(output_path, "w+") as f:
        f.write('''\
#pragma once

#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

''')
        for comp in components:
            f.write('''\
#include "Components/{component_name}.h"
'''.format(component_name = comp))

        f.write('''\

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

''')
        for comp in components:
            f.write('''\
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::{component_name}>(registry);
'''.format(component_name = comp))

        f.write("}\n")

def analyse_components():
    struct_pattern = re.compile(r'\s*struct\s+(\w+)')
    field_pattern  = re.compile(r'^\s*(\w[\w\s*&:<>]*?)\s+(\w+)\b')

    braces_level = 0
    for componentLines in get_component_files(ROOT_SEARCH_DIR):
        struct_name         = None
        member_variables    = []
        for line in componentLines:
            if not struct_name:
                match:  Optional[Match[str]] = struct_pattern.search(line)
                if match is not None:
                    struct_name = match.group(1)
                    print("Struct: " + struct_name)
            else:
                match = field_pattern.search(line)
                if line.find('{') != -1:
                    braces_level = braces_level + 1
                if line.find('}') != -1:
                    braces_level = braces_level - 1
                if match and braces_level <= 1:
                    if line.find("(") != -1 and line.find(")") != -1:
                        if line.find("inline") != -1 or ((line.find("(") < line.find("{")) and line.find("{") != -1):
                            continue

                    member_type, member_name = match.groups()
                    member_variables.append((member_type, member_name))
                    print("Field: " + member_name + " -> [" + member_type + "]")

        components.append(struct_name)

        generate_rtti_for_component(struct_name, member_variables)

analyse_components()

register_components()
import os
import re
from sys import argv
from typing import Optional, Match

from ScarletTypes import types
from StringTemplates import (
    GENERATE_PROPERTIES_TEMPLATE,
    REGISTER_COMPONENT_HEADER_TEMPLATE,
    REGISTER_COMPONENT_SOURCE_TEMPLATE,
    GENERATE_PROPERTY_TEMPLATE,
    END_TEMPLATE
)

COMPONENT_MACRO = "SCARLET_COMPONENT("

components = []

# Function to find all files that have the C++ macro 'SCARLET_COMPONENT()'
def get_component_files(path):
    result = []
    for root, dirs, files in os.walk(path):
        for file in files:
            filepath = os.path.join(root, file)

            if not filepath.endswith(".h"):
                continue

            with open(filepath, "r") as f:
                content = f.read()
                if COMPONENT_MACRO in content:
                    result.append(content.splitlines())
    return result

# Generate RTTI for each component's member variables
def generate_rtti_for_component(generated_path, struct_name, member_variables):
    os.makedirs(generated_path, exist_ok=True)
    output_path = os.path.join(generated_path, f"{struct_name}.generated.cpp")

    # Filter out member variables whose type is not in the 'types' dictionary
    filtered_member_variables = [
        variable for variable in member_variables
        if types.get(variable[0]) is not None
    ]

    # Prepare the RTTI property string for valid types
    property_strings = [GENERATE_PROPERTY_TEMPLATE.format(property_name=variable[1],
                                                          type_enum=types.get(variable[0], "Unknown"),
                                                          struct_name=struct_name)
                        for variable in filtered_member_variables]

    # Join the property strings with new lines
    property_string = "\n".join(property_strings)

    # Write the generated RTTI code to a file
    with open(output_path, "w+") as f:
        f.write(GENERATE_PROPERTIES_TEMPLATE.format(struct_name=struct_name))
        f.write(property_string)
        f.write(END_TEMPLATE)

# Register components in the generated header file
def register_components(generated_path, external_components):
    os.makedirs(generated_path, exist_ok=True)
    header_output_path = os.path.join(generated_path, "Register.generated.h")
    source_output_path = os.path.join(generated_path, "Register.generated.cpp")

    with open(header_output_path, "w+") as f:
        component_include_string = ""
        for comp in components:
            if external_components:
                component_include_string += f'#include "Components/{comp}.h"\n'
            else:
                component_include_string += f'#include "ScarletCoreEcs/Components/{comp}.h"\n'

        f.write(REGISTER_COMPONENT_HEADER_TEMPLATE.format(component_includes=component_include_string))

    with open(source_output_path, "w+") as f:
        component_registry_string   = ""
        component_unregistry_string = ""
        for comp in components:
            component_registry_string   += f'    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::{comp}>(registry);\n'
            component_unregistry_string += f'    UnregisterComponentType<Scarlet::Component::{comp}>(registry);\n'
        f.write(REGISTER_COMPONENT_SOURCE_TEMPLATE.format(components_registry=component_registry_string, components_unregistry=component_unregistry_string))


# Analyze components in source files
def analyse_components(components_search_path):
    struct_pattern = re.compile(r'^\s*struct\s+(?:\w+\s+)*(\w+)')
    field_pattern  = re.compile(r'^\s*(\w[\w\s*&:<>]*?)\s+(\w+)\b')

    braces_level = 0
    for component_lines in get_component_files(components_search_path):
        struct_name = None
        member_variables = []

        for line in component_lines:
            # Find struct names
            if not struct_name:
                match: Optional[Match[str]] = struct_pattern.search(line)
                if match:
                    struct_name = match.group(1)
                    print(f"Struct: {struct_name}")
            else:
                match = field_pattern.search(line)

                # Track braces to handle nested code
                if '{' in line:
                    braces_level += 1
                if '}' in line:
                    braces_level -= 1

                # Process variables when braces level is appropriate
                if match and braces_level <= 1:
                    # Skip function-like or inline declarations
                    if "(" in line and ")" in line and ("inline" in line or line.find("{") < line.find("(")):
                        continue

                    member_type, member_name = match.groups()
                    member_variables.append((member_type, member_name))
                    print(f"Field: {member_name} -> [{member_type}]")

        components.append(struct_name)
        generate_rtti_for_component(os.path.join(components_search_path, "Generated/"), struct_name, member_variables)

# Run the analysis and component registration

engine_path  = argv[1]
project_path = argv[2]

# ================= Engine Components.
engine_component_search_path = os.path.join(engine_path, "Source/ScarletCoreEcsProject/ScarletCoreEcs/Components/")

analyse_components(engine_component_search_path)
register_components(os.path.join(engine_component_search_path, "Generated/"), external_components = False)

# Reset the found components.
components = []

# ================= Project Components.
project_component_search_path = os.path.join(project_path, "Components/")

analyse_components(project_component_search_path)
register_components(os.path.join(project_component_search_path, "Generated/"), external_components = True)
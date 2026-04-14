import os
import shutil
import xml.etree.ElementTree as et
from sys import argv

from ScarletTypes import reverse_types
from Entity import Entity
from Component import Component
from Property import Property

project_name = argv[1]
project_path = argv[2]
engine_path  = argv[3]
build_path   = argv[4]

build_output_directory = os.path.join(build_path, project_name)

# If the build path already exists, nuke it to prevent old build information.
if os.path.exists(build_output_directory):
    shutil.rmtree(build_output_directory)

tree = et.parse(os.path.join(project_path,"Content/Scenes/GameScene.scarlet_scene"))

scene_element = tree.getroot()

entities = []

for entity_element in scene_element:
    ent = Entity()

    # Iterate over components
    for component_element in entity_element.findall("Component"):
        component = Component()
        component.type = component_element.attrib["typeId"]

        # If this is editor only, grab the information required and don't add it as its editor only.
        if component.type == "EditorInfo":
            mutableElements = component_element.findall("isMutable");
            if len(mutableElements) == 0:
                ent.mutable = False
            else:
                ent.mutable = mutableElements[0].text == "true"
            continue

        for component_properties in component_element:
            property_type = component_properties.attrib["type"]
            component.properties.append(Property(property_type, component_properties.tag, component_properties.text))
            component.contains_asset_reference |= property_type == "asset"

        ent.components.append(component)

    entities.append(ent)

project_build_directory = os.path.join(project_path, "Core", "Build")

# Ensure the build directory exists.
os.makedirs(project_build_directory, exist_ok=True)

with open(os.path.join(project_build_directory, "SceneBuild.h"), 'w+') as f:
    f.write('''\
#pragma once

#ifndef DEV_CONFIGURATION

#include <ScarletCore/Defines.h>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include <ScarletEngine/Core/Filepath.h>

#include <ScarletCoreEcs/Components/Generated/Register.generated.h>

namespace Scarlet
{{

inline void InitGameScene()
{{
    Filepath::SetEngineDirectory("{engine_asset_path}");
    Filepath::SetProjectDirectory("{project_asset_path}");

    Engine::Instance().LoadScarletAssets();
    
    WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().GetScene(\"{scene_name}\");
'''.format(engine_asset_path=os.path.join(build_output_directory,"EngineResources").replace('\\', '/'),
           project_asset_path=os.path.join(build_output_directory,"GameResources").replace('\\', '/'),
           scene_name="GameScene"))
    for e in entities:
        if e.mutable:
            f.write("\n    {\n        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();\n")
            for c in e.components:
                f.write("        auto& {component} = ent.AddComponent<Component::{component}>();\n".format(component=c.type))
                for p in c.properties:
                    f.write(f"        {c.type}.{p.name} = {reverse_types[p.type.upper()]}{{ {p.value} }};\n")
            f.write("    }\n")
        else: # Not mutable.
            f.write("\n   {\n")
            add_entity_parameter = ""
            component_template_types = ""
            for c in e.components:
                if add_entity_parameter != "":
                    add_entity_parameter += ", "
                add_entity_parameter += f"std::move({c.type})"

                if component_template_types != "":
                    component_template_types += ", "
                component_template_types += f"Component::{c.type}"

                f.write("        Component::{component} {component}{{}};\n".format(component=c.type))
                for p in c.properties:
                    f.write("        {component}.{property_name} = {property_type}{{ {property_value} }};\n"
                            .format(component=c.type, property_name=p.name, property_type=reverse_types[p.type.upper()], property_value=p.value))
            f.write("\n        scene->AddEntity<{component_template}>({component_parameters});".format(component_template=component_template_types, component_parameters=add_entity_parameter))
            f.write("\n    }\n")
    f.write('''
}

} // Namespace Scarlet.

#endif // DEV_CONFIGURATION.

''')

# Final build directory structure.
# Output directory name
# |-> Engine DLL's
# |-> Game DLL
# |-> Executable
# |-> EngineResources
# |--> EngineAssets
# |-> GameResources
# |--> GameAssets.

directories = [
    os.path.join(build_output_directory, "EngineResources"),
    os.path.join(build_output_directory, "GameResources", "GameAssets"),
]

[os.makedirs(d, exist_ok=True) for d in directories]

# Copy Engine Assets to build path.
shutil.copytree(os.path.join(engine_path, "EngineAssets"), os.path.join(build_output_directory, "EngineResources", "EngineAssets"), dirs_exist_ok=True)

# Copy Engine DLL's.
shutil.copy(os.path.join(engine_path, "Build", "Bin", "ScarlEnt.dll"), build_output_directory)
shutil.copy(os.path.join(engine_path, "Build", "Bin", "ScarletEngine.dll"), build_output_directory)
shutil.copy(os.path.join(engine_path, "Build", "Bin", "Scarlet.exe"), os.path.join(build_output_directory, f"{project_name}.exe"))
shutil.copy(os.path.join(project_path, "Scratch", "Bin", "Release", project_name, f"{project_name}.dll"), os.path.join(build_output_directory, "Game.dll"))

# Copy Game Assets.
shutil.copytree(os.path.join(project_path, "Content"), os.path.join(build_output_directory, "GameResources", "GameAssets"), dirs_exist_ok=True)
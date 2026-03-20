import xml.etree.ElementTree as et

from ScarletTypes import reverse_types
from Entity import Entity
from Component import Component
from Property import Property

path = "E:/Programming/ScarletEngine/ScarletProjects/ScarletTestGameProject/Content/Scenes/ScarletTestGameProject.scarlet_scene"
tree = et.parse(path)

scene_element = tree.getroot()

entities = []

for entity_element in scene_element:
    ent = Entity()
    ent.mutable = entity_element.attrib["Mutable"] == "true"
    for component_element in entity_element.findall("Component"):
        component = Component()
        component.type = component_element.attrib["typeId"]
        for component_properties in component_element:
            property_type = component_properties.attrib["type"]
            component.properties.append(Property(property_type, component_properties.tag, component_properties.text))
            component.contains_asset_reference |= property_type == "asset"
        ent.components.append(component)
    entities.append(ent)

with open("E:/Programming/ScarletEngine/ScarletProjects/ScarletTestGameProject/Core/Build/SceneBuild.h", 'w+') as f:
    f.write('''\
#pragma once

#ifndef DEV_CONFIGURATION

#include <ScarletCore/Defines.h>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include <ScarletCoreEcs/Components/Generated/Register.generated.h>

namespace Scarlet
{{

inline void InitGameScene()
{{
    WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().GetScene(\"{scene_name}\");
    ScarlEnt::Registry::Instance().SetActiveScene(scene);
'''.format(scene_name="ScarletTestGameProject"))
    for e in entities:
        if e.mutable:
            f.write('''\

    {
        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
''')
            for c in e.components:
                if True:#c.contains_asset_reference:
                    f.write('''\
        auto& {component} = ent.AddComponent<Component::{component}>();
'''.format(component=c.type))
                    for p in c.properties:
                        f.write(f'''\
        {c.type}.{p.name} = {reverse_types[p.type.upper()]}{{ {p.value} }};
''')
                else:
                    component_arguments = ""
                    for p in c.properties:
                        if component_arguments != "":
                            component_arguments += ", "
                        component_arguments += f"{reverse_types[p.type.upper()]} {{ {p.value} }}"
                    f.write('''\
        ent.AddComponent<Component::{component}>({component_arguments});

'''.format(component=c.type, component_arguments=component_arguments))
            f.write('''\
    }
''')

    f.write('''
}

} // Namespace Scarlet.

#endif // DEV_CONFIGURATION.

''')
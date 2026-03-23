import sys

def create_project_files(project_name, project_path, starting_scene_name):
    # =========== Premake File.
    with (open(project_path + "/" + project_name + ".scarlet_project", "w+") as f):
        f.write("<ScarletProject>\n\t<DefaultScene relativePath=\"Content/Scenes/" + starting_scene_name +".scarlet_scene\" />\n</ScarletProject>")

    # =========== Premake File.
    with (open("E:/Programming/ScarletEngine/EngineAssets/ProjectTemplates/premake5.lua.scarlet_template", "r") as f):
        file_template_str = f.read()

    with open(project_path + "/premake5.lua", "w+") as f:
        f.write(file_template_str.format(project_name=project_name, project_path=project_path))

    # =========== Defines.h
    with (open("E:/Programming/ScarletEngine/EngineAssets/ProjectTemplates/Defines.h.scarlet_template", "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Defines.h", "w+") as f:
        f.write(file_template_str)

    # =========== Game.h
    with (open("E:/Programming/ScarletEngine/EngineAssets/ProjectTemplates/Game.h.scarlet_template", "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Game.h", "w+") as f:
        f.write(file_template_str)

    # =========== Game.cpp
    with (open("E:/Programming/ScarletEngine/EngineAssets/ProjectTemplates/Game.cpp.scarlet_template", "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Game.cpp", "w+") as f:
        f.write(file_template_str.format(starting_scene_name=starting_scene_name))

    # =========== StartingScene.h
    with (open("E:/Programming/ScarletEngine/EngineAssets/ProjectTemplates/StartingScene.h.scarlet_template", "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Scenes/" + starting_scene_name + ".h", "w+") as f:
        f.write(file_template_str.format(starting_scene_name=starting_scene_name))

    # =========== StartingScene.scarlet_scene
    with (open("E:/Programming/ScarletEngine/EngineAssets/ProjectTemplates/StartingScene.scarlet_scene.scarlet_template", "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Content/Scenes/" + starting_scene_name + ".scarlet_scene", "w+") as f:
        f.write(file_template_str)

create_project_files(sys.argv[1], sys.argv[2], sys.argv[3])

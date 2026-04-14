import os
import sys

def create_project_files(engine_path, project_name, project_path, starting_scene_name):
    project_template_path = os.path.join(engine_path, "EngineAssets", "ProjectTemplates")

    # =========== Premake File.
    with (open(project_path + "/" + project_name + ".scarlet_project", "w+") as f):
        f.write("<ScarletProject>\n\t<DefaultScene relativePath=\"Content/Scenes/" + starting_scene_name +".scarlet_scene\" />\n</ScarletProject>")

    # =========== Premake File.
    with (open(os.path.join(project_template_path, "premake5.lua.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path + "/premake5.lua", "w+") as f:
        f.write(file_template_str.format(project_name=project_name, project_path=project_path, engine_path=engine_path))

    # =========== Defines.h
    with (open(os.path.join(project_template_path, "Defines.h.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Defines.h", "w+") as f:
        f.write(file_template_str)

    # =========== Game.h
    with (open(os.path.join(project_template_path, "Game.h.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Game.h", "w+") as f:
        f.write(file_template_str)

    # =========== Game.cpp
    with (open(os.path.join(project_template_path, "Game.cpp.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Game.cpp", "w+") as f:
        f.write(file_template_str.format(starting_scene_name=starting_scene_name))

    # =========== StartingScene.h
    with (open(os.path.join(project_template_path, "StartingScene.h.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Core/Scenes/" + starting_scene_name + ".h", "w+") as f:
        f.write(file_template_str.format(starting_scene_name=starting_scene_name))

    # =========== StartingScene.scarlet_scene
    with (open(os.path.join(project_template_path, "StartingScene.scarlet_scene.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path + "/Content/Scenes/" + starting_scene_name + ".scarlet_scene", "w+") as f:
        f.write(file_template_str)

    # =========== GenerateSolution.bat
    with (open(os.path.join(project_template_path, "GenerateSolution.bat.scarlet_template"), "r") as f):
        file_template_str = f.read()

    with open(project_path, "w+") as f:
        f.write(file_template_str.formate(engine_path=engine_path))

# Arguments passed in: 1: EnginePath, 2: ProjectName, 3: ProjectPath, 4: StartingSceneName
create_project_files(sys.argv[1].replace('\\','/'), sys.argv[2], sys.argv[3], sys.argv[4])

-- ---------------------------- ScarletEditor Project.
project "ScarletEditor"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    pchheader "ScarletEditorPch.h"
    pchsource "%{prj.name}/Src/ScarletEditorPch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}",
        "%{prj.name}/Src/",

        "$(SolutionDir)Deps/Include/",

        "$(SolutionDir)Source/ScarlEntProject/",
        "$(SolutionDir)Source/ScarletMathProject/",
        "$(SolutionDir)Source/ScarletCoreProject/",
        "$(SolutionDir)Source/ScarletEngineProject/",
        "$(SolutionDir)Source/ScarletReflectProject/",
        "$(SolutionDir)Source/ScarletCoreEcsProject/",
        "$(SolutionDir)Source/ScarletLoggerProject/",

        "$(SolutionDir)Deps/ImGui/",
        "$(SolutionDir)Deps/ImGuizmo/",
    }

    libdirs
    {
        "$(SolutionDir)Deps/Lib"
    }

    links
    {
        "ScarlEnt",
        "ScarletMath",
        "ScarletCore",
        "ScarletEngine",
        "ScarletReflect",
        "ScarletCoreEcs",
        "ScarletLogger",

        "ImGui",
        "ImGuizmo",

        "glfw3_mt.lib",
        "glew32s.lib",
        "opengl32.lib"
    }

    postbuildcommands
    {
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "DEV_CONFIGURATION"
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        optimize "On"

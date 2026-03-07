-- ---------------------------- ScarletEditor Project.
project "ScarletEditor"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Deps/Include/",

        "$(SolutionDir)Source/ScarletEditorProject/ScarletEditor",

        "$(SolutionDir)Source/ScarlEntProject/",
        "$(SolutionDir)Source/ScarletMathProject/",
        "$(SolutionDir)Source/ScarletCoreProject/",
        "$(SolutionDir)Source/ScarletEngineProject/",
        "$(SolutionDir)Source/ScarletLoggerProject/",

        "$(SolutionDir)Deps/ImGui/",
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
        "ScarletLogger",

        "ImGui",

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

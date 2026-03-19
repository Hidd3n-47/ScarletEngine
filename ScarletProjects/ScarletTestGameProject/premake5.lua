-- ---------------------------- ScarletTestGameProject Project.
project "ScarletTestGameProject"
    kind "SharedLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    files
    {
        "**.h",
        "**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Deps/Include/",

        "$(SolutionDir)Source/ScarletProject/",
        "$(SolutionDir)Source/ScarletProject/src/",

        "$(SolutionDir)Source/ScarlEntProject/",
        "$(SolutionDir)Source/ScarletMathProject/",
        "$(SolutionDir)Source/ScarletCoreProject/",
        "$(SolutionDir)Source/ScarletCoreEcsProject/",
        "$(SolutionDir)Source/ScarletEngineProject/",
    }

    libdirs
    {
    }

    links
    {
        "ScarlEnt",
        "ScarletMath",
        "ScarletCore",
        "ScarletCoreEcs",
        "ScarletEngine"
    }

    postbuildcommands
    {
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "DEV_CONFIGURATION", "GAME_DLL_BUILD" }
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        defines "GAME_DLL_BUILD"
        optimize "On"

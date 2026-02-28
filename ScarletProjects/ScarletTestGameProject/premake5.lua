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
        "$(SolutionDir)Source/ScarletProject/",
    }

    libdirs
    {
    }

    links
    {
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

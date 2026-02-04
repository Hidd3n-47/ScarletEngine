-- ---------------------------- ScarletEngine Project.
project "ScarletEngine"
    location "%{prj.name}"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    pchheader "ScarletEnginePch.h"
    pchsource "%{prj.name}/Src/ScarletEnginePch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/Src/",

        "$(SolutionDir)Source/ScarletCoreProject/",

        "$(SolutionDir)Deps/Include/"
    }

    libdirs
    {
        "$(SolutionDir)Deps/Lib/"
    }

    links
    {
        "glfw3_mt.lib"
    }

    postbuildcommands
    {
        --'{MKDIR} "' .. outputPath .. '/Scarlet/"',
        --'{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. outputPath .. '/Scarlet/"'
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

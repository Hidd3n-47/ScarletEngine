-- ---------------------------- ScarletCore Project.
project "ScarletCore"
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
        "$(SolutionDir)Source/ScarletCoreProject/",

        "$(SolutionDir)Source/ScarletLoggerProject/",
    }

    libdirs
    {
    }

    links
    {
        "ScarletLogger",
    }

    postbuildcommands
    {
        '{MKDIR} "$(SolutionDir)Bin/"',
        '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Bin/"'
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

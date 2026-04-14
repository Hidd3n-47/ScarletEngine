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

    links
    {
        "ScarletLogger",
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "DEV_CONFIGURATION"
        symbols "On"
        postbuildcommands
        {
            '{MKDIR} "$(SolutionDir)Bin/"',
            '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Bin/"'
        }

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        postbuildcommands
        {
            '{MKDIR} "$(SolutionDir)Build/Bin/"',
            '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Build/Bin/"'
        }

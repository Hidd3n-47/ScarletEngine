-- ---------------------------- Scarlet Project.
project "Scarlet"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    pchheader "ScarletPch.h"
    pchsource "%{prj.name}/Src/ScarletPch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/",

        "$(SolutionDir)Source/ScarletLoggerProject/",
        "$(SolutionDir)Source/ScarletCoreProject/",
        "$(SolutionDir)Source/ScarletMathProject/",
        "$(SolutionDir)Source/ScarletEngineProject/",
        "$(SolutionDir)Source/ScarlEntProject/",
    }

    links
    {
        "ScarletCore",
        "ScarletEngine",
        "ScarletEditor"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines "DEV_CONFIGURATION"
        symbols "On"
        includedirs { "$(SolutionDir)Source/ScarletEditorProject/" }

    filter "configurations:Release"
        runtime "Release"
        optimize "On"
        postbuildcommands
        {
            '{MKDIR} "$(SolutionDir)Build/Bin/"',
            '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.exe" "$(SolutionDir)Build/Bin/"'
        }

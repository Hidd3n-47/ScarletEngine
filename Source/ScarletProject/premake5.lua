-- ---------------------------- Scarlet Project.
project "Scarlet"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    pchheader "ScarletPch.h"
    pchsource "%{prj.name}/Src/ScarletPch.cpp"

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/Src/",

        "$(SolutionDir)Source/ScarletCore/",
        "$(SolutionDir)Source/ScarletEngine/",
    }

    libdirs
    {
    }

    links
    {
        "ScarletCore",
        "ScarletEngine",
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

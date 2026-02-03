-- ---------------------------- ScarletCore Project.
project "ScarletCore"
    location "%{prj.name}"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"

    targetdir(outputPath .. "%{prj.name}")
    objdir(outputIntPath .. "%{prj.name}")

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "%{prj.name}/Src/",
    }

    libdirs
    {
    }

    links
    {
    }

    postbuildcommands
    {
        '{MKDIR} "' .. outputPath .. 'Scarlet"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. outputPath .. 'Scarlet/"'
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

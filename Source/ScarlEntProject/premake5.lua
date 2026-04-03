-- ---------------------------- ScarlEnt Project.
project "ScarlEnt"
    location "%{prj.name}"
    kind "SharedLib"
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
        "%{prj.name}",
        "%{prj.name}/Src/",

        "$(SolutionDir)Source/ScarlEntProject/",
        "$(SolutionDir)Source/ScarletCoreProject/",

        "$(SolutionDir)Deps/Include/"
    }

    libdirs
    {
    }

    links
    {
        "ScarletCore"
    }

    postbuildcommands
    {
        '{MKDIR} "' .. testOutputPath .. '/ScarlEntTest/"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. testOutputPath .. '/ScarlEntTest/"',

        '{MKDIR} "' .. outputPath .. '/Scarlet/"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. outputPath .. '/Scarlet/"',

        '{MKDIR} "$(SolutionDir)Bin/"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "$(SolutionDir)Bin/"',
        '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Bin/"'
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "DEV_CONFIGURATION", "SCARLENT" }
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLENT"
        optimize "On"

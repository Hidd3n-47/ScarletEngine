-- ---------------------------- ScarletCoreEcs Project.
project "ScarletCoreEcs"
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
        "",
        "%{prj.name}",
        "%{prj.name}/Src/",

        "$(SolutionDir)/Deps/Include",

        "$(SolutionDir)Source/ScarlEntProject/",
        "$(SolutionDir)Source/ScarletMathProject/",
        "$(SolutionDir)Source/ScarletCoreProject/",
        "$(SolutionDir)Source/ScarletReflectProject/",
        "$(SolutionDir)Source/ScarletLoggerProject/",
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
        "ScarletReflect",
        "ScarletLogger",
    }

    postbuildcommands
    {
        '{MKDIR} "' .. outputPath .. '/Scarlet/"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. outputPath .. '/Scarlet/"',

        '{MKDIR} "$(SolutionDir)Bin/"',
        '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Bin/"'
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "COMPONENTS_BUILD", "DEV_CONFIGURATION"}
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        defines "COMPONENTS_BUILD"
        optimize "On"

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

-- ---------------------------- ScarletEngine Project.
project "ScarletEngine"
    location "%{prj.name}"
    kind "SharedLib"
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
        "%{prj.name}",
        "%{prj.name}/Src/",

        "$(SolutionDir)Source/ScarletCoreProject/",
        "$(SolutionDir)Source/ScarletMathProject/",
        "$(SolutionDir)Source/ScarletLoggerProject/",
        "$(SolutionDir)Source/ScarletEngineProject/",
        "$(SolutionDir)Source/ScarletReflectProject/",
        "$(SolutionDir)Source/ScarletCoreEcsProject/",
        "$(SolutionDir)Source/ScarlEntProject/",

        "$(SolutionDir)Deps/ImGui/",
        "$(SolutionDir)Deps/ImGuizmo/",

        "$(SolutionDir)Deps/Include/",
    }

    libdirs
    {
        "$(SolutionDir)Deps/Lib/"
    }

    links
    {
        "ScarletCore",
        "ScarletMath",
        "ScarletLogger",
        "ScarletCoreEcs",
        "ScarlEnt",

        "ImGui",
        "ImGuizmo",

        "glfw3_mt.lib",
        "glew32s.lib",
        "opengl32.lib"
    }

    postbuildcommands
    {
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
        defines { "ENGINE_BUILD", "DEV_CONFIGURATION", "SCARLET_ENGINE_PATH=R\"($(SolutionDir))\"" }
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        defines "ENGINE_BUILD"
        optimize "On"

-- ---------------------------- ScarletMath project.
project "ScarletMath"
    location "%{prj.name}"
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
        "$(SolutionDir)Source/ScarletMathProject/",

        "$(SolutionDir)Deps/include/",
    }

    postbuildcommands
    {
        --("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettGameCore/]"),
        --("{COPYFILE} %[%{cfg.buildtarget.abspath}] %[" .. outputPath .."ScarlettEditor/]")
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        defines "SCARLETT_MATH_DEBUG"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        defines "SCARLETT_MATH_RELEASE"
        optimize "On"
        runtime "Release"

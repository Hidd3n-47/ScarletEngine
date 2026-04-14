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

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        defines "SCARLETT_MATH_DEBUG"
        symbols "On"
        runtime "Debug"
        postbuildcommands
        {
            '{MKDIR} "$(SolutionDir)Bin/"',
            '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Bin/"'
        }

    filter "configurations:Release"
        defines "SCARLETT_MATH_RELEASE"
        optimize "On"
        runtime "Release"
        postbuildcommands
        {
            '{MKDIR} "$(SolutionDir)Build/Bin/"',
            '{COPYFILE} "%{cfg.targetdir}/%{prj.name}.lib" "$(SolutionDir)Build/Bin/"'
        }

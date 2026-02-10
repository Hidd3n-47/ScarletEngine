-- ---------------------------- ScarletMathTests project.
project "ScarletMathTests"
    location "%{prj.name}"
    kind "ConsoleApp"
    language "C++"
    staticruntime "Off"
    cppdialect "C++20"
    
    targetdir(path.join(testOutputPath, "%{prj.name}"))
    objdir(path.join(testOutputIntPath, "%{prj.name}"))

    files
    {
        "%{prj.name}/**.h",
        "%{prj.name}/**.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Tests/ScarletTestTemplateProject/",
        "$(SolutionDir)Tests/ScarletMathTestProject/",

        "$(SolutionDir)Deps/Include/",
        "$(SolutionDir)Source/ScarletMathProject/",
    }

    links
    {
       "ScarletTestTemplate",
       "ScarletMath"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "on"
        runtime "Debug"

    filter "configurations:Release"
        optimize "on"
        runtime "Release"

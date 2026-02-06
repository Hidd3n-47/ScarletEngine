-- ---------------------------- ScarlEntTest project.
project "ScarlEntTest"
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
        "$(SolutionDir)Tests/ScarlEntTestProject/",

        "$(SolutionDir)Source/ScarlentProject/",

        "$(SolutionDir)Deps/Include/",
    }

    libdirs
    {
        path.join(outputPath, "ScarlEnt")
    }

    links
    {
       "ScarletTestTemplate",
       "ScarlEnt"
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
-- ---------------------------- ScarletTestTemplate project.
project "ScarletTestTemplate"
    location "%{prj.name}"
    kind "StaticLib"
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

        "$(SolutionDir)Source/ScarletLoggerProject/"
    }

    links
    {
        "ScarletLogger"
    }

    postbuildcommands
    {
        --'{MKDIR} "' .. testOutputPath .. '/ScarlEntTest/"',
        --'{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. testOutputPath .. '/ScarlEntTest/"'
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

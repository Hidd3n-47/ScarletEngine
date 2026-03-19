-- ---------------------------- ScarletAssetManage Project.
project "ScarletAssetManager"
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
        '{MKDIR} "' .. outputPath .. '/Scarlet/"',
        '{COPYFILE} "%{cfg.buildtarget.abspath}" "' .. outputPath .. '/Scarlet/"'
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        runtime "Debug"
        defines { "DEV_CONFIGURATION", "SCARLET_ASSET_MANAGER" }
        symbols "On"

    filter "configurations:Release"
        runtime "Release"
        defines "SCARLET_ASSET_MANAGER"
        optimize "On"

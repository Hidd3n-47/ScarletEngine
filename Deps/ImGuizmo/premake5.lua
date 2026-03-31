-- ---------------------------- ImGuizmo project.
project "ImGuizmo"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++17"
    
    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    files
    {
        "ImGuizmo/*.h",
        "ImGuizmo/*.cpp",
    }

    includedirs
    {
        "$(SolutionDir)Deps/Include/",

        "$(SolutionDir)Deps/imgui/imgui/"
    }

    links
    {
        "imgui"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        optimize "On"
        runtime "Release"

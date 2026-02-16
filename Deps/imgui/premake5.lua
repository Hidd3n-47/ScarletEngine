-- ---------------------------- imgui project.
project "ImGui"
    kind "StaticLib"
    language "C++"
    staticruntime "Off"
    cppdialect "C++17"
    
    targetdir(path.join(outputPath, "%{prj.name}"))
    objdir(path.join(outputIntPath, "%{prj.name}"))

    files
    {
        "imgui/*.h",
        "imgui/*.cpp",

        "imgui/backends/imgui_impl_opengl3.h",
        "imgui/backends/imgui_impl_opengl3.cpp",
        "imgui/backends/imgui_impl_opengl3_loader.h",
    }

    includedirs
    {
        "$(SolutionDir)Deps/Include/",

        "$(SolutionDir)Deps/imgui/imgui/"
    }

    removefiles
    {
        --"%{prj.name}/backends/**.h",
        --"%{prj.name}/backends/**.cpp"
    }

    filter "system:windows"
        systemversion "latest"

    filter "configurations:Dev"
        symbols "On"
        runtime "Debug"

    filter "configurations:Release"
        optimize "On"
        runtime "Release"

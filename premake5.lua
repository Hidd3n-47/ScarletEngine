workspace "Scarlet"
architecture "x64"

configurations
{
    "Dev",
    "Release"
}

outputName        = "%{cfg.buildcfg}"
outputPath        = path.join(_MAIN_SCRIPT_DIR, "Scratch", "Bin", outputName)
outputIntPath     = path.join(_MAIN_SCRIPT_DIR, "Scratch", "Bin-Int", outputName)
testOutputPath    = path.join(_MAIN_SCRIPT_DIR, "Scratch", "Bin", outputName, "Tests")
testOutputIntPath = path.join(_MAIN_SCRIPT_DIR, "Scratch", "Bin-Int", outputName, "Tests")


-- Ensure that paths are absolute paths.
outputPath    = path.getabsolute(outputPath)
outputIntPath = path.getabsolute(outputIntPath)

include "Source/ScarletProject/"
include "Source/ScarlEntProject/"
include "Source/ScarletCoreProject/"
include "Source/ScarletMathProject/"
include "Source/ScarletLoggerProject/"
include "Source/ScarletEngineProject/"

-- Test Projects
include "Tests/ScarletTestTemplateProject/"
include "Tests/ScarletMathTestProject/"
include "Tests/ScarlEntTestProject/"

-- Dependency Projects.
include "Deps/ImGui/"

-- Project.
include "ScarletProjects/ScarletTestGameProject/"
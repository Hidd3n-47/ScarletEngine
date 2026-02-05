workspace "Scarlet"
architecture "x64"

configurations
{
    "Dev",
    "Release"
}

outputName    = "%{cfg.buildcfg}"
outputPath    = path.join(_MAIN_SCRIPT_DIR, "Scratch", "Bin", outputName)
outputIntPath = path.join(_MAIN_SCRIPT_DIR, "Scratch", "Bin-Int", outputName)

-- Ensure that paths are absolute paths.
outputPath    = path.getabsolute(outputPath)
outputIntPath = path.getabsolute(outputIntPath)

include "Source/ScarletProject/"
include "Source/ScarlEntProject/"
include "Source/ScarletCoreProject/"
include "Source/ScarletEngineProject/"

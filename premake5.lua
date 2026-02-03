workspace "Scarlet"
architecture "x64"

configurations
{
    "Dev",
    "Release"
}

outputName    = "%{cfg.buildcfg}"
outputPath    = "%{wks.location}Scratch/Bin/" .. outputName .. "/"
outputIntPath = "%{wks.location}Scratch/Bin-Int/" .. outputName .. "/"

include "Source/ScarletProject/"
include "Source/ScarletCoreProject/"
include "Source/ScarletEngineProject/"

#include "ScarletEnginePch.h"
#include "Time.h"

#include <chrono>

namespace Scarlet
{

double Time::mSimulationFrameRate    = 0.0;
uint64 Time::mPreviousTimeSinceEpoch = 0;
uint64 Time::mTargetFrameTime        = 0;

void Time::Init(const double simulationFrameRate)
{
    mSimulationFrameRate = simulationFrameRate;
    mTargetFrameTime     = static_cast<uint64>(1'000.0 / simulationFrameRate);
}

bool Time::Tick()
{
    const uint64 currentTime = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count();

    if (currentTime - mPreviousTimeSinceEpoch < mTargetFrameTime)
    {
        return false;
    }

    mPreviousTimeSinceEpoch = currentTime;
    return true;
}

} // Namespace Scarlet.
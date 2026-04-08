#pragma once

#include "Defines.h"

namespace Scarlet
{

/**
 * @class Time: A static class responsible for handling time and fixed update.
 */
class ENGINE_API Time
{
public:
    /**
     * @brief Initialise and setup time class to have a fixed simulation frame rate.
     * @param simulationFrameRate The number of frames in the fixed update. By default, set to 120.
     */
    static void Init(const double simulationFrameRate = 120.0);
    /**
     * @brief Update the clock and find out if enough time has elapsed for the fixed update to occur.
     * @return \c true if enough time has elapsed for the fixed update to occur, \c false otherwise
     */
    [[nodiscard]] static bool Tick();

    [[nodiscard]] inline static double GetFrameDelta() { return static_cast<double>(mFrameDeltaMs) / 1'000; }
    /**
     * @brief Get the delta time in seconds of the fixed update.
     * @return The fixed delta time in seconds.
     */
    [[nodiscard]] inline static double GetFixedFrameDelta() { return static_cast<double>(mTargetFrameTime) / 1'000; }
private:
    static double mSimulationFrameRate;
    static uint64 mPreviousFixedUpdateTimeSinceEpochMs;
    static uint64 mPreviousFrameTimeSinceEpochMs;
    static uint64 mTargetFrameTime;
    static uint64 mFrameDeltaMs;
};

} // Namespace Scarlet.

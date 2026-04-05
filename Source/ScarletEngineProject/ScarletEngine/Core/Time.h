#pragma once

namespace Scarlet
{

/**
 * @class Time: A static class responsible for handling time and fixed update.
 */
class Time
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

    /**
     * @brief Get the delta time in seconds of the fixed update.
     * @return The fixed delta time in seconds.
     */
    [[nodiscard]] inline static double GetFixedFrameDelta() { return 1'000.0 / static_cast<double>(mTargetFrameTime); }
private:
    static double mSimulationFrameRate;
    static uint64 mPreviousTimeSinceEpoch;
    static uint64 mTargetFrameTime;
};

} // Namespace Scarlet.
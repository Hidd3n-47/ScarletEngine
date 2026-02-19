#include "Ulid.h"

#include <chrono>
#include <random>

namespace Scarlet
{

namespace
{
std::random_device   gDevice;
std::mt19937         gRandomEngine{ gDevice() };
std::uniform_int_distribution<uint32> gUniformDistribution;
} // Anonymous namespace.

Ulid::Ulid()
{
    const uint32 time = GetTimeFromEpoch();
    const uint32 rand = GetRandomNumber();

    mUlid = static_cast<uint64>(time) << 32 | rand;
}

uint32 Ulid::GetTimeFromEpoch()
{
    using namespace std::chrono;
    constexpr sys_days epoch = 2026y / 1 / 1;

    const time_point now = floor<seconds>(system_clock::now());
    return static_cast<int>((now - epoch) / 1s);
}

uint32 Ulid::GetRandomNumber()
{
    return gUniformDistribution(gRandomEngine);
}

} //Namespace Scarlet.

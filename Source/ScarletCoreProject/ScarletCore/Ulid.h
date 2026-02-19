#pragma once

#include <unordered_set>

#include "PrimitiveTypes.h"

namespace Scarlet
{
/**
 * @class Ulid: A unique 64-bit number. The first 32-bits represent a 32-bit random number. \\n
 * The remaining 32-bits represent the timestamp of creation from a custom epoch.\\n
 * (time_since_epoch << 32) | (random_number)
 */
class Ulid
{
public:
    /**
     * @brief Construct a new ULID.
     */
    Ulid();
    /**
     * @brief Create a ULID with the passed in 64-bit number value. This should only be used when creating a ULID for a serialised ULID.
     * @param ulid The 64-bit number representing the ULID.
     */
    inline constexpr explicit Ulid(const uint64 ulid) : mUlid{ ulid } { /* Empty. */ }

    inline operator uint64() const
    {
        return mUlid;
    }

    [[nodiscard]] inline bool operator==(const Ulid& other) const { return mUlid == other.mUlid; }
private:
    uint64 mUlid;

    /**
     * @brief Get the time from a custom epoch.
     * @return The time since the custom epoch.
     */
    [[nodiscard]] static uint32 GetTimeFromEpoch();

    /**
     * @brief Get a random 32-bit number.
     * @return A random 32-bit number.
     */
    [[nodiscard]] static uint32 GetRandomNumber();
};

} // Namespace Scarlett.

template <>
struct std::hash<Scarlet::Ulid>
{
    [[nodiscard]] inline size_t operator()(const Scarlet::Ulid& ulid) const noexcept
    {
        return std::hash<uint64>()(ulid);
    }
};
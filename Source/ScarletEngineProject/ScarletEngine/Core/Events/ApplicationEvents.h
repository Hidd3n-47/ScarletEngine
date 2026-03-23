#pragma once

#include "Event.h"

namespace Scarlet
{

/**
 * @class WindowClosedEvent: A class used to represent a window closed event.
 */
class WindowClosedEvent final : public Event
{
public:
    EVENT_TYPE(WINDOW_CLOSE)
};

class WindowResizedEvent final : public Event
{
public:
    inline WindowResizedEvent(const uint32 width, const uint32 height) : mWidth{ width }, mHeight{ height } { /* Empty. */}

    [[nodiscard]] inline uint32 GetWidth()  const { return mWidth; }
    [[nodiscard]] inline uint32 GetHeight() const { return mHeight; }

    EVENT_TYPE(WINDOW_RESIZED)
private:
    uint32 mWidth, mHeight;
};

} // Namespace Scarlet.

#pragma once

#include "Event.h"

namespace Scarlet
{

/**
 * @class MouseEvent: A class used to represent a mouse event.
 */
class MouseEvent : public Event
{
public:
    inline explicit MouseEvent(const float x, const float y) noexcept
        : mX(x), mY(y)
    { /* Empty. */ }

    [[nodiscard]] inline float GetXPos() const { return mX; }
    [[nodiscard]] inline float GetYPos() const { return mY; }
protected:
    float mX = 0.0f;
    float mY = 0.0f;
};

/**
 * @class MouseMovedEvent: A class used to represent a mouse moved event.
 */
class MouseMovedEvent final : public MouseEvent
{
public:
    inline explicit MouseMovedEvent(const float x, const float y) noexcept
        : MouseEvent{ x, y }
    { /* Empty. */ }

    EVENT_TYPE(MOUSE_MOVE)
};

/**
 * @class MouseScrollEvent: A class used to represent a mouse scroll event.
 */
class MouseScrollEvent final : public Event
{
public:
    inline explicit MouseScrollEvent(const float x, const float y) noexcept
        : mXOffset{ x }, mYOffset{ y }
    { /* Empty. */ }

    [[nodiscard]] inline float GetXOffset() const { return mXOffset; }
    [[nodiscard]] inline float GetYOffset() const { return mYOffset; }

    EVENT_TYPE(MOUSE_SCROLL)
private:
    float mXOffset = 0.0f;
    float mYOffset = 0.0f;
};

/**
 * @class MouseButtonEvent: A class used to represent a mouse button event.
 */
class MouseButtonEvent : public Event
{
public:
    inline explicit MouseButtonEvent(const uint32 button) noexcept
        : mButton{ button }
    { /* Empty. */ }

    [[nodiscard]] inline uint32 GetMouseButton() const { return mButton; }
private:
    uint32 mButton;
};

/**
 * @class MouseButtonPressedEvent: A class used to represent a mouse button pressed event.
 */
class MouseButtonPressedEvent final : public MouseButtonEvent
{
public:
    inline explicit MouseButtonPressedEvent(const uint32 button) noexcept
        : MouseButtonEvent{ button }
    { /* Empty. */ }

    EVENT_TYPE(MOUSE_BUTTON_PRESSED)
};

/**
 * @class MouseButtonReleasedEvent: A class used to represent a mouse button released event.
 */
class MouseButtonReleasedEvent final : public MouseButtonEvent
{
public:
    inline explicit MouseButtonReleasedEvent(const uint32 button) noexcept
        : MouseButtonEvent{ button }
    { /* Empty. */ }

    EVENT_TYPE(MOUSE_BUTTON_RELEASED)
};

} // Namespace Scarlet.
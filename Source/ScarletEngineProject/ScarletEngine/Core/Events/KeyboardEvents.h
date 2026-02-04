#pragma once

#include "Event.h"

namespace Scarlet
{

/**
 * @class KeyEvent: A class used to represent an event related to a keyboard action.
 */
class KeyEvent : public Event
{
public:
    inline explicit KeyEvent(const uint32 keyCode) noexcept
        : mKeyCode{ keyCode }
    { /* Empty. */ }

    [[nodiscard]] inline uint32 GetKeyCode() const { return mKeyCode; }
private:
    uint32 mKeyCode;
};

/**
 * @class KeyPressedEvent: A class used to represent a key pressed event.
 */
class KeyPressedEvent final : public KeyEvent
{
public:
    inline explicit KeyPressedEvent(const uint32 keyCode) noexcept
        : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_TYPE(KEY_PRESSED)
};

/**
 * @class KeyRepeatEvent: A class used to represent a key repeated event. Repeated happens once the key is held down.
 */
class KeyRepeatEvent final : public KeyEvent
{
public:
    inline explicit KeyRepeatEvent(const uint32 keyCode) noexcept
        : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_TYPE(KEY_REPEAT)
};

/**
 * @class KeyReleasedEvent: A class used to represent a key released event.
 */
class KeyReleasedEvent final : public KeyEvent
{
public:
    inline explicit KeyReleasedEvent(const uint32 keyCode) noexcept
        : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_TYPE(KEY_RELEASED)
};

/**
 * @class KeyTypedEvent: A class used to represent a key typed event.
 */
class KeyTypedEvent final : public KeyEvent
{
public:
    inline explicit KeyTypedEvent(const uint32 keyCode) noexcept
        : KeyEvent{ keyCode }
    { /* Empty. */ }

    EVENT_TYPE(KEY_TYPED)
};

} // Namespace Scarlet.
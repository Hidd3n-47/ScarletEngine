#pragma once

namespace Scarlet
{

/**
 * @enum EventType: An enum to represent the type of event.
 */
enum class EventType
{
    WINDOW_CLOSE,

    MOUSE_MOVE,
    MOUSE_SCROLL,
    MOUSE_BUTTON_PRESSED,
    MOUSE_BUTTON_RELEASED,

    KEY_PRESSED,
    KEY_REPEAT,
    KEY_RELEASED,
    KEY_TYPED
};


#define EVENT_TYPE(type) [[nodiscard]] static  EventType GetStaticType()                { return EventType::##type; } \
                         [[nodiscard]] virtual EventType GetEventType () const override { return GetStaticType(); }

/**
* @class Event: A class that represents an event.
* @ref EventType
*/
class Event
{
    friend class EventDispatcher;
public:
    Event() noexcept          = default;
    virtual ~Event() noexcept = default;

    Event(const Event&)            = default;
    Event(Event&&)                 = default;
    Event& operator=(Event&&)      = default;
    Event& operator=(const Event&) = default;

    /**
     * @brief Get the type of event @ref EventType.
     * @return The type of the event.
     */
    [[nodiscard]] virtual EventType GetEventType() const = 0;
    /**
     * @brief Get if the event is handled or not.
     * @return \c true if the event is handled, \c false otherwise.
     */
    [[nodiscard]] inline bool IsHandled() const { return mHandled; }
protected:
    bool mHandled = false;
};

/**
 * @class EventDispatcher: A class used to dispatch events to their respective bound function.
 */
class EventDispatcher
{
public:
    inline explicit EventDispatcher(Event& event) noexcept
        : mEvent{ &event }
    { /* Empty*/ }

    /**
     * @brief Dispatch an event class to a respective function callback.
     * @tparam T Generic interface of the events where \c T inherits from \c Event.
     * @ref Event
     * @param function The callback function the event is being dispatched to.
     * @return \c true if the event was successfully dispatched to a callback method, \c false otherwise.
     */
    template<typename T>
    inline bool Dispatch(const std::function<bool(const T&)>& function) noexcept
    {
        if (mEvent->GetEventType() == T::GetStaticType())
        {
            mEvent->mHandled = function(*static_cast<T*>(mEvent));
            return true;
        }

        return false;
    }

private:
    Event* mEvent;
};

} // Namespace Scarlet.

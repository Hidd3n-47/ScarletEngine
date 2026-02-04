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

} // Namespace Scarlet.

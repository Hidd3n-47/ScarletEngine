#include "ScarletEnginePch.h"
#include "InputManager.h"

#include "Core/Events/Event.h"
#include "Core/Events/MouseEvents.h"
#include "Core/Events/KeyboardEvents.h"

namespace Scarlet
{

InputManager::KeyMap InputManager::mKeysDownThisFrame   = { };
InputManager::KeyMap InputManager::mKeyMap              = { };
Math::Vec2           InputManager::mMouseDeltaThisFrame = { 0.0f, 0.0f };
Math::Vec2           InputManager::mMousePosition       = { 0.0f, 0.0f };

void InputManager::OnEvent(Event& e)
{
    EventDispatcher dispatcher(e);


    dispatcher.Dispatch<KeyPressedEvent>([](const KeyPressedEvent& event) {
        const uint32 keyCode = event.GetKeyCode();
        if (!mKeyMap.contains(keyCode)) { mKeysDownThisFrame[keyCode] = true; }
        mKeyMap[keyCode] = true;
        return false;
    });

    dispatcher.Dispatch<KeyReleasedEvent>([](const KeyReleasedEvent& event) {
        mKeyMap[event.GetKeyCode()] = false;
        return false;
    });

    dispatcher.Dispatch<MouseButtonPressedEvent>([](const MouseButtonPressedEvent& event) {
        mKeyMap[event.GetMouseButton()] = true;
        return false;
    });

    dispatcher.Dispatch<MouseButtonReleasedEvent>([](const MouseButtonReleasedEvent& event) {
        mKeyMap[event.GetMouseButton()] = false;
        return false;
    });

    dispatcher.Dispatch<MouseMovedEvent>([](const MouseMovedEvent& event) {
        const Math::Vec2 previousPosition = mMousePosition;
        mMousePosition     = { event.GetXPos(), event.GetYPos() };
        mMouseDeltaThisFrame = previousPosition - mMousePosition;
        return false;
    });
}

void InputManager::ResetInput()
{
    mKeysDownThisFrame.clear();
    mMouseDeltaThisFrame = Math::Vec2{};
}

} // Namespace Scarlet.
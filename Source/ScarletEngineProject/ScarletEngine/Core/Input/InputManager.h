#pragma once

#include <ScarletMath/Math.h>

namespace Scarlet
{

class Event;

/**
 * @class InputManager: A static class to store the keyboard and mouse state.
 */
class InputManager
{
    friend class Engine;
    using KeyMap = unordered_map<uint32, bool>;
public:
    /**
     * @brief Get if a key was pressed down this frame.
     * @param keyCode The \ref KeyCode that is being requested if it's down or not.
     * @return \c true if the key was pressed down this frame, \c false otherwise.
     */
    [[nodiscard]] static inline bool IsKeyPressed(const uint16 keyCode) { return mKeysDownThisFrame[keyCode]; }
    /**
     * @brief Get if a key is pressed down.
     * @param keyCode The \ref KeyCode that is being requested if it's down or not.
     * @return \c true if the key is down, \c false otherwise.
     */
    [[nodiscard]] static inline bool IsKeyDown(const uint16 keyCode) { return mKeyMap[keyCode]; }
    /**
     * @brief Get if a mouse button is pressed down.
     * @param mouseCode The \ref KeyCode that is being requested if it's down or not.
     * @return \c true if the key is down, \c false otherwise.
     */
    [[nodiscard]] static inline bool IsMouseButtonDown(const uint16 mouseCode) { return mKeyMap[mouseCode]; }
    /**
     * @brief Get the current mouse position in screenspace.
     * @return The mouse position (screenspace).
     */
    [[nodiscard]] static inline Math::Vec2 GetMousePosition() { return mMousePosition; }
    /**
     * @brief Get the mouse delta from the previous frame to the current frame.
     * @return The mouse delta from the previous frame to the current frame.
     */
    [[nodiscard]] static inline Math::Vec2 GetMouseDeltaThisFrame() { return mMouseDeltaThisFrame; }
private:
    static KeyMap mKeysDownThisFrame;
    static KeyMap mKeyMap;

    static Math::Vec2 mMouseDeltaThisFrame;
    static Math::Vec2 mMousePosition;

    /**
     * @brief Internal function used to handle and dispatch the input events.
     * @param e The \ref Event.
     * @note This function does not "handle" the events, it is purely taking a look at them to store the keyboard and mouse states.
     */
    static void OnEvent(Event& e);
    /**
     * @brief Internal function to reset variables tracking states for single frames.
     */
    static void ResetInput();
};

} // Namespace Scarlet.
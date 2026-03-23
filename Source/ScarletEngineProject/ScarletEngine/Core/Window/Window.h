#pragma once

#include "ScarletEngine/Core/Defines.h"

struct GLFWwindow;

namespace Scarlet
{

class Event;

/**
 * @struct WindowProperties
 * A structure to hold the properties of a window.
 */
struct WindowProperties
{
    /** @brief The width of the window.           */
    uint32 width         = 1920;
    /** @brief The height of the window.          */
    uint32 height        = 1080;
    /** @brief If the window is resizable.        */
    bool   resizable     = false;
    /** @brief If the window contains a titlebar. */
    bool   titleBar      = true;
    /** @brief Create the current OpenGL context. */
    bool   makeGlContext = true;

    /** A function pointer to a callback method used to process events. */
    std::function<void(Event&)> eventCallback{};
};

/**
 * @class Window
 * A class to hold a handle to a window that allows for window interaction that is API agnostic.\\n
 * Use @ref WindowManager to create and destroy a window.
 */
class ENGINE_API Window
{
public:
    Window(GLFWwindow* windowHandle, WindowProperties&& windowProperties);

    /**
     * @brief Update the window.
     */
    void Update() const;

    /**
     * @brief Set the position of the window. Both -1 on \c positionX and \c positionY will center the window.
     * @param positionX The position relative to the x-axis.
     * @param positionY The position relative to the y-axis.
     */
    void SetPosition(const int positionX, const int positionY) const;
    /**
     * @brief Set the width and height of the window.
     * @param width The new width of the window.
     * @param height The new height of the window.
     */
    void SetSize(const uint32 width, const uint32 height) const;
    /**
     * @brief Set the title of the window.
     * @param title The new title of the window.
     */
    void SetTitle(const char* title) const;
    /**
     * @brief Get the width of the window.
     * @return The width of the window.
     */
    [[nodiscard]] inline uint32 GetWidth()        const    { return mWindowProperties.width; }
    /**
     * @brief Get the height of the window.
     * @return The height of the window.
     */
    [[nodiscard]] inline uint32 GetHeight()       const    { return mWindowProperties.height; }
    /**
     * @brief Get a handle to the native window.
     * @return A pointer to the raw memory of the window.
     */
    [[nodiscard]] inline void*  GetNativeWindow() const    { return mWindowHandle; }
    /**
     * @brief Get the properties of the window.
     * @ref WindowProperties
     * @return Properties of the window.
     */
    [[nodiscard]] inline WindowProperties* GetProperties() { return &mWindowProperties; }

    /**
     * @brief Set the function pointer for event callback.
     */
    inline void SetEventCallback(const std::function<void(Event&)>& callback) { mWindowProperties.eventCallback = callback; }
private:
    GLFWwindow*      mWindowHandle;
    WindowProperties mWindowProperties;
};

} // Namespace Scarlet.

#pragma once

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
    WindowProperties()  = default;
    ~WindowProperties() = default;

    WindowProperties(const WindowProperties&)            = default;
    WindowProperties(WindowProperties&&)                 = default;
    WindowProperties& operator=(WindowProperties&&)      = default;
    WindowProperties& operator=(const WindowProperties&) = default;

    /** The width of the window.           */
    uint32 width         = 1920;
    /** The height of the window.          */
    uint32 height        = 1080;
    /** If the window is resizable.        */
    bool   resizable     = false;
    /** If the window contains a titlebar. */
    bool   titleBar      = true;
    /** Create the current OpenGL context. */
    bool   makeGlContext = true;

    /** A function pointer to a callback method used to process events. */
    std::function<void(Event&)> eventCallback;
};

/**
 * @class Window
 * A class to hold a handle to a window that allows for window interaction that is API agnostic.\\n
 * Use @ref WindowManager to create and destroy a window.
 */
class Window
{
public:
    Window(GLFWwindow* windowHandle, WindowProperties&& windowProperties);

    /**
     * @brief Update the window.
     */
    void Update() const;

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

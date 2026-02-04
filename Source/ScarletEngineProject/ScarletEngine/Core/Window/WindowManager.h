#pragma once

#include "Window.h"

namespace Scarlet
{

/**
 * @class WindowManager: A class used to manage the creation and destruction of windows
 * as well as the initialisation and termination of the API used to create windows.
 */
class WindowManager
{
public:
    /**
     * @brief Initialize the API used to create windows.<br/>
     * @note: API must be initialized before a window can be made or destroyed.
     */
    static void InitApi();

    /**
     * @brief Terminate the API used to create windows.
     */
    static void TerminateApi();

    /**
     * @brief Create a window.
     * @param title The title/caption of the window.
     * @param windowProperties The properties of the created window.
     * @see WindowProperties
     * @see WeakHandle
     * @return A handle to the created window.
     */
    static WeakHandle<Window> CreateWindowInternal(const char* title, const WindowProperties& windowProperties = {});

    /**
     * @brief Destroy a window.
     * @param window The handle to widow that is being destroyed. <br/>
     * Note the window handle will be invalidated after being destroyed.
     */
    static void DestroyWindow(WeakHandle<Window>& window);

private:
    static bool mInitialized;
};

} // Namespace Scarlett.

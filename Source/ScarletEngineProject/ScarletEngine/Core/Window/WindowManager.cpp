#include "ScarletEnginePch.h"
#include "WindowManager.h"

#include <glfw/glfw3.h>

#include "Core/Events/MouseEvents.h"
#include "Core/Events/KeyboardEvents.h"
#include "Core/Events/ApplicationEvents.h"

namespace Scarlet
{

namespace
{

void GlfwErrorCallback(int error, const char* description)
{
    __debugbreak();
}

template <typename T, typename ...Args>
void DispatchEvent(GLFWwindow* win, Args... args)
{
    const WindowProperties& data = *static_cast<WindowProperties*>(glfwGetWindowUserPointer(win));

    if (!data.eventCallback)
    {
        return;
    }

    T event{ std::forward<Args>(args)... };
    data.eventCallback(event);
}

} // Anonymous namespace.

bool WindowManager::mInitialized = false;

void WindowManager::InitApi()
{
    const int glfwOkay = glfwInit();
    glfwSetErrorCallback(GlfwErrorCallback);

    SCARLET_ASSERT(glfwOkay == GLFW_TRUE && "Failed to initialise GLFW.");

    mInitialized = true;
}

void WindowManager::ApiPoll()
{
    glfwPollEvents();
}

void WindowManager::TerminateApi()
{
    glfwTerminate();
}

WeakHandle<Window> WindowManager::CreateWindowInternal(const char* title, WindowProperties&& windowProperties /* = {} */)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_RESIZABLE, windowProperties.resizable);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowProperties.width), static_cast<int>(windowProperties.height), title, nullptr, nullptr);

    SCARLET_ASSERT(window && "Failed to create GLFW window.");

    if (windowProperties.makeGlContext)
    {
        glfwMakeContextCurrent(window);
    }

    Window* windowHandle = new Window(window, std::move(windowProperties));
    glfwSetWindowUserPointer(window, windowHandle->GetProperties());

    glfwSetWindowCloseCallback(window, [](GLFWwindow* win) { DispatchEvent<WindowClosedEvent>(win); });

    glfwSetCursorPosCallback(window, 
        [](GLFWwindow* win, const double x, const double y) { DispatchEvent<MouseMovedEvent>(win, static_cast<float>(x), static_cast<float>(y)); });

    glfwSetScrollCallback(window, 
        [](GLFWwindow* win, const double xOffset, const double yOffset) { DispatchEvent<MouseScrollEvent>(win, static_cast<float>(xOffset), static_cast<float>(yOffset)); });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, const int button, const int action, const int mods)
        {
            switch (action)
            {
            case GLFW_PRESS:
            {
                DispatchEvent<MouseButtonPressedEvent>(win, static_cast<uint32>(button));
                break;
            }
            case GLFW_RELEASE:
            {
                DispatchEvent<MouseButtonReleasedEvent>(win, static_cast<uint32>(button));
                break;
            }
            default:
                SCARLET_WARN("Unhandled Mouse Action Event. GLFW action: {}", action);
                break;
            }
        });

    glfwSetKeyCallback(window, [](GLFWwindow* win, const int key, const int scanCode, const int action, const int mods)
        {
            switch (action)
            {
            case GLFW_PRESS:
            {
                DispatchEvent<KeyPressedEvent>(win, static_cast<uint32>(key));
                break;
            }
            case GLFW_RELEASE:
            {
                DispatchEvent<KeyReleasedEvent>(win, static_cast<uint32>(key));
                break;
            }
            case GLFW_REPEAT:
            {
                DispatchEvent<KeyTypedEvent>(win, static_cast<uint32>(key));
                break;
            }
            default:
                SCARLET_WARN("Unhandled Key Action Event. GLFW action: {}", action);
                break;
            }
        });

    glfwSetCharCallback(window,
        [](GLFWwindow* win, const uint32 keycode){ DispatchEvent<KeyTypedEvent>(win, keycode); });

    return WeakHandle{ windowHandle };
}

void WindowManager::DestroyWindow(WeakHandle<Window>& window)
{
    glfwDestroyWindow(static_cast<GLFWwindow*>(window.GetRawPtr()->GetNativeWindow()));

    delete window.GetRawPtr();
    window.Invalidate();
}

} // Namespace Scarlet.

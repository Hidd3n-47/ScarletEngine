#include "ScarletEnginePch.h"
#include "WindowManager.h"

#include <glfw/glfw3.h>

namespace Scarlet
{

namespace
{

void GlfwErrorCallback(int error, const char* description)
{
    __debugbreak();
}

} // Anonymous namespace.

bool WindowManager::mInitialized = false;

void WindowManager::InitApi()
{
    mInitialized = true;

    (void)glfwInit();
    glfwSetErrorCallback(GlfwErrorCallback);
}

void WindowManager::TerminateApi()
{
    glfwTerminate();
}

WeakHandle<Window> WindowManager::CreateWindowInternal(const char* title, const WindowProperties& windowProperties /* = {} */)
{
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API);
    glfwWindowHint(GLFW_RESIZABLE, windowProperties.resizable);

    GLFWwindow* window = glfwCreateWindow(static_cast<int>(windowProperties.width), static_cast<int>(windowProperties.height), title, nullptr, nullptr);

    Window* windowHandle = new Window(window, windowProperties);
    glfwSetWindowUserPointer(window, windowHandle->GetProperties());

    return WeakHandle{ windowHandle };
}

void WindowManager::DestroyWindow(WeakHandle<Window>& window)
{
    glfwDestroyWindow(static_cast<GLFWwindow*>(window.GetRawPtr()->GetNativeWindow()));

    delete window.GetRawPtr();
    window.Invalidate();
}

} // Namespace Scarlet.

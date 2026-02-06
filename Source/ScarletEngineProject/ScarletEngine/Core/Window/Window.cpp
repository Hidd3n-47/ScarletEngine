#include "ScarletEnginePch.h"
#include "Window.h"

#include <glfw/glfw3.h>

namespace Scarlet
{

Window::Window(GLFWwindow* windowHandle, WindowProperties&& windowProperties)
    : mWindowHandle(windowHandle)
    , mWindowProperties(std::move(windowProperties))
{
    // Empty.
}

void Window::Update() const
{
    glfwSwapBuffers(mWindowHandle);
}

} // Namespace Scarlet.
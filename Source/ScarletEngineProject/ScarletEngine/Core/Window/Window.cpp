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

void Window::SetPosition(const int positionX, const int positionY) const
{
    if (positionX == positionY && positionX == -1)
    {
        GLFWmonitor* monitor    = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        glfwSetWindowPos(mWindowHandle, (mode->width - static_cast<int>(mWindowProperties.width)) >> 1, (mode->height - static_cast<int>(mWindowProperties.height)) >> 1);
        return;
    }

    glfwSetWindowPos(mWindowHandle, positionX, positionY);
}

void Window::SetSize(const uint32 width, const uint32 height) const
{
    glfwSetWindowSize(mWindowHandle, static_cast<int>(width), static_cast<int>(height));
}

void Window::SetTitle(const char* title) const
{
    glfwSetWindowTitle(mWindowHandle, title);
}

} // Namespace Scarlet.
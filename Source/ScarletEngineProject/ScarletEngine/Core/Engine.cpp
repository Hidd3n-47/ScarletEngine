#include "ScarletEnginePch.h"
#include "Engine.h"

#define GLEW_STATIC
#include <glew/glew.h>

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

#include "Window/WindowManager.h"

namespace Scarlet
{

void Engine::CreateEngine()
{
    mInstance = new Engine();
}

void Engine::Init()
{
    DEBUG(Log::Init());

    WindowManager::InitApi();
    mMainWindow = WindowManager::CreateWindowInternal("Scarlet Engine");
    mMainWindow->SetEventCallback([](Event& e) { Instance().OnEvent(e); });

    const unsigned int glewOkay = glewInit();
    SCARLET_ASSERT(glewOkay == GLEW_OK && "Failed to initialise glew!");

    mRunning = true;
    SCARLET_INFO("Engine Initialised!");
}

void Engine::Destroy()
{
    WindowManager::DestroyWindow(mInstance->mMainWindow);
    WindowManager::TerminateApi();

    delete mInstance;
    SCARLET_INFO("Engine Destroyed!");
}

void Engine::Run() const
{
    while (mRunning)
    {
        WindowManager::ApiPoll();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glBegin(GL_TRIANGLES);
        glVertex2f(-0.5f, -0.5f);
        glVertex2f(0.0f, 0.5f);
        glVertex2f(0.5f, -0.5f);
        glEnd();

        mMainWindow->Update();
    }
}

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent&) { mRunning = false; return true; });
}

} // Namespace Scarlet.
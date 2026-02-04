#include "ScarletEnginePch.h"
#include "Engine.h"

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
    WindowManager::InitApi();
    mInstance->mMainWindow = WindowManager::CreateWindowInternal("Scarlet Engine");
    mInstance->mMainWindow->SetEventCallback([](Event& e) { Instance().OnEvent(e); });

    mInstance->mRunning = true;
}

void Engine::Destroy()
{
    WindowManager::DestroyWindow(mInstance->mMainWindow);
    WindowManager::TerminateApi();

    delete mInstance;
}

void Engine::Run() const
{
    while (mRunning)
    {
        mMainWindow->Update();
    }
}

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent&) { mRunning = false; return true; });
}

} // Namespace Scarlet.
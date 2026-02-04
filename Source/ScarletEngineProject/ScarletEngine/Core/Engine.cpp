#include "ScarletEnginePch.h"
#include "Engine.h"

#include "Window/WindowManager.h"

namespace Scarlet
{

void Engine::Init()
{
    mInstance = new Engine();

    WindowManager::InitApi();
    mInstance->mMainWindow = WindowManager::CreateWindowInternal("Scarlet Engine");

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
        
    }
}

} // Namespace Scarlet.
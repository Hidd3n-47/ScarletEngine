#include "ScarletEnginePch.h"
#include "Engine.h"

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

#include "Input/InputManager.h"

#include "Window/WindowManager.h"

#include "Rendering/Renderer.h"

#include "AssetLoading/AssetManager.h"

#ifdef DEV_CONFIGURATION
#include "Components/Generated/Register.generated.h"
#endif // DEV_CONFIGURATION.

namespace Scarlet
{

void Engine::CreateEngine() noexcept
{
    SCARLET_ASSERT(!mInstance && "Trying to create an instance of the engine one already exists.");
    mInstance = new Engine();
}

void Engine::Init() noexcept
{
    DEBUG(Log::Init());

    WindowManager::InitApi();

    WindowProperties windowProperties{ .eventCallback = [](Event& e) { Instance().OnEvent(e); } };
    DEBUG(windowProperties.resizable = true);
    mMainWindow = WindowManager::CreateWindowInternal("Scarlet Engine", std::move(windowProperties));

    ScarlEnt::Registry::Init();

    Renderer::InitApi();

    DEBUG(RegisterComponents());

    mAssetManager = new AssetManager();
    mAssetManager->LoadScarletAssets(Filepath{ FilepathDirectory::ENGINE, "EngineAssets/" });

    mRunning = true;
    SCARLET_INFO("Engine Initialised!");
}

void Engine::Destroy() const noexcept
{
    SCARLET_ASSERT(!mRunning && "Trying to destroy engine whilst Engine::Run is running.");

    delete mAssetManager;

    Renderer::TerminateApi();

    ScarlEnt::Registry::Terminate();

    WindowManager::DestroyWindow(mInstance->mMainWindow);
    WindowManager::TerminateApi();

    delete mInstance;
    mInstance = nullptr;
    SCARLET_INFO("Engine Destroyed!");
}

void Engine::Run() const
{
    while (mRunning)
    {
        InputManager::ResetInput();

        WindowManager::ApiPoll();

        ScarlEnt::Registry::Instance().GetActiveScene()->Update();

        DEBUG(if (mBeginRenderEvent) mBeginRenderEvent());

        Renderer::Instance().Render();
        DEBUG(if (mEndRenderEvent) mEndRenderEvent());

        mMainWindow->Update();

        ScarlEnt::Registry::Instance().PostUpdate();
    }
}

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent&) { mRunning = false; return true; });

    InputManager::OnEvent(event);
}

} // Namespace Scarlet.
#include "ScarletEnginePch.h"
#include "Engine.h"

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

#include "Window/WindowManager.h"

#include "Rendering/Renderer.h"

#include "Rendering/Mesh.h"
#include "Rendering/Texture.h"

namespace Scarlet
{

void Engine::CreateEngine() noexcept
{
    mInstance = new Engine();
}

void Engine::Init() noexcept
{
    DEBUG(Log::Init());

    WindowManager::InitApi();
    mMainWindow = WindowManager::CreateWindowInternal("Scarlet Engine");
    mMainWindow->SetEventCallback([](Event& e) { Instance().OnEvent(e); });

    Renderer::InitApi(mMainWindow);

    mUvMapTexture = new Resource::Texture{ "E:/Programming/ScarletEngine/EngineAssets/uvMap.png" };

    mCube   = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Cube.obj" };
    mMonkey = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Monkey.obj" };
    mCone   = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Cone.obj" };

    mRunning = true;
    SCARLET_INFO("Engine Initialised!");
}

void Engine::Destroy() noexcept
{
    SCARLET_ASSERT(!mRunning && "Trying to destroy engine whilst Engine::Run is running.");
    Renderer::TerminateApi();

    WindowManager::DestroyWindow(mInstance->mMainWindow);
    WindowManager::TerminateApi();

    delete mInstance;
    SCARLET_INFO("Engine Destroyed!");
}

void Engine::Run() const
{
    Math::Mat4 cubePos{ 1.0f };
    Math::Mat4 cubePos1{ 1.0f };
    Math::Mat4 otherPos{ 1.0f };
    Math::Mat4 otherPos1{ 1.0f };
    Math::Mat4 conePos{ 1.0f };
    cubePos[3][0]   = 5.0f;
    cubePos[3][1]   = 0.0f;
    cubePos[3][2]   = 2.0f;
    cubePos1[3][2]  = 2.5f;
    otherPos[3][0]  = -2.5f;
    otherPos1[3][2] = -2.5f;
    conePos[3][0]   = 6.0f;

    while (mRunning)
    {
        WindowManager::ApiPoll();

        Renderer::Instance().AddRenderCommand(Resource::Material{ WeakHandle{ mUvMapTexture } }, WeakHandle{ mCube }, cubePos);
        Renderer::Instance().AddRenderCommand(Resource::Material{ WeakHandle{ mUvMapTexture } }, WeakHandle{ mCube }, cubePos1);
        Renderer::Instance().AddRenderCommand(Resource::Material{ WeakHandle{ mUvMapTexture } }, WeakHandle{ mMonkey }, otherPos);
        Renderer::Instance().AddRenderCommand(Resource::Material{ WeakHandle{ mUvMapTexture } }, WeakHandle{ mMonkey }, otherPos1);
        Renderer::Instance().AddRenderCommand(Resource::Material{ WeakHandle{ mUvMapTexture } }, WeakHandle{ mCone }, conePos);
        Renderer::Instance().Render();

        mMainWindow->Update();
    }
}

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent&) { mRunning = false; return true; });
}

} // Namespace Scarlet.
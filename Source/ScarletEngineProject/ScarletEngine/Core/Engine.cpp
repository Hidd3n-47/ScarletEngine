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
    mGlockTexture = new Resource::Texture{ "E:/Programming/ScarletEngine/EngineAssets/Glock17_BaseColor.png" };

    mCube   = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Cube.obj" };
    mMonkey = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Monkey.obj" };
    mCone   = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Cone.obj" };
    mGlock  = new Resource::Mesh{ "E:/Programming/ScarletEngine/EngineAssets/Glock17.obj" };

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
    Math::Mat4 cubeFloor{ 1.0f };
    cubeFloor[2][3] = -2.0f;
    cubeFloor[0][0] =  8.0f;
    cubeFloor[1][1] =  8.0f;
    cubeFloor[2][2] =  1.0f;

    Math::Mat4 cubePos  { 1.0f };
    cubePos[0][3] = 5.0f;
    cubePos[1][3] = 0.0f;
    cubePos[2][3] = 2.0f;

    Math::Mat4 cubePos1 { 1.0f };
    cubePos1[0][3] = -2.5f;
    cubePos1[2][3] =  2.5f;

    Math::Mat4 monkeyPos{ 1.0f };
    monkeyPos[0][3] = -2.5f;

    Math::Mat4 monkeyPos1{ 1.0f };
    monkeyPos1[0][3] = 2.5f;
    monkeyPos1[2][3] = 5.5f;

    Math::Mat4 conePos{ 1.0f };
    conePos[0][3] = 6.0f;

    Math::Mat4 glockPos{ 1.0f };
    glockPos[0][0] = 20.0f;
    glockPos[1][1] = 20.0f;
    glockPos[2][2] = 20.0f;
    glockPos[2][3] = 2.0f;

    Resource::Material uvMaterial{ WeakHandle{ mUvMapTexture }, { 1.0f, 1.0f, 1.0f } };
    Resource::Material glockMaterial{ WeakHandle{ mGlockTexture }, { 1.0f, 1.0f, 1.0f } };

    while (mRunning)
    {
        WindowManager::ApiPoll();

        Renderer::Instance().AddRenderCommand(WeakHandle{ &uvMaterial }, WeakHandle{ mCube }  , cubeFloor);
        Renderer::Instance().AddRenderCommand(WeakHandle{ &uvMaterial }, WeakHandle{ mCube }  , cubePos);
        Renderer::Instance().AddRenderCommand(WeakHandle{ &uvMaterial }, WeakHandle{ mCube }  , cubePos1);
        Renderer::Instance().AddRenderCommand(WeakHandle{ &uvMaterial }, WeakHandle{ mMonkey }, monkeyPos);
        Renderer::Instance().AddRenderCommand(WeakHandle{ &uvMaterial }, WeakHandle{ mMonkey }, monkeyPos1);
        Renderer::Instance().AddRenderCommand(WeakHandle{ &uvMaterial }, WeakHandle{ mCone }, conePos);
        Renderer::Instance().AddRenderCommand(WeakHandle{ &glockMaterial }, WeakHandle{ mGlock }  , glockPos);
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
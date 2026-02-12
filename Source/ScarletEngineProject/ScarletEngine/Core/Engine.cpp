#include "ScarletEnginePch.h"
#include "Engine.h"

#define GLEW_STATIC
#include <glew/glew.h>

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

#include "Window/WindowManager.h"

#include "Rendering/Shader.h"
#include "Rendering/Renderer.h"
#include "Rendering/VertexArray.h"
#include "Rendering/IndexBuffer.h"
#include "Rendering/VertexBuffer.h"
#include "Rendering/BufferLayout.h"

#include "Rendering/MeshData.h"
#include "Rendering/MeshLoader.h"

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

    Renderer::InitApi();

    mRunning = true;
    SCARLET_INFO("Engine Initialised!");
}

void Engine::Destroy() noexcept
{
    SCARLET_ASSERT(!mRunning && "Trying to destroy engine whilst Engine::Run is running.");

    WindowManager::DestroyWindow(mInstance->mMainWindow);
    WindowManager::TerminateApi();

    delete mInstance;
    SCARLET_INFO("Engine Destroyed!");
}

void Engine::Run() const
{
    float positions[] = { -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f };
    uint32 indices[] = { 0, 1, 2, 2, 3, 0};

    Resource::MeshData cube;
    MeshLoader::LoadMesh("E:/Programming/ScarletEngine/EngineAssets/Cube.obj", cube);

    const VertexArray va;
    VertexBuffer vb(cube.vertices.data(), cube.vertices.size() * sizeof(Resource::Vertex));
    vb.PushLayoutElement<float>(3);
    vb.PushLayoutElement<float>(3);
    vb.PushLayoutElement<float>(2);
    va.AddBuffer(vb);

    const IndexBuffer ib(cube.indices.data(), cube.indices.size());

    const Shader shader("E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.vert", "E:/Programming/ScarletEngine/EngineAssets/Shaders/editor.frag");

    while (mRunning)
    {
        WindowManager::ApiPoll();

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        va.Bind();
        ib.Bind();
        shader.Bind();

        glDrawElements(GL_TRIANGLES, cube.indices.size(), GL_UNSIGNED_INT, nullptr);

        mMainWindow->Update();
    }
}

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent&) { mRunning = false; return true; });
}

} // Namespace Scarlet.
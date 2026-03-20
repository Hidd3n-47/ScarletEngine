#include "ScarletEnginePch.h"
#include "Engine.h"

#include <ScarletMath/Trig.h>

#include <ScarlEnt/Scene.h>

#include <ScarletCoreEcs/Components/Mesh.h>
#include <ScarletCoreEcs/Components/Transform.h>

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

#include "Input/InputManager.h"

#include "Window/WindowManager.h"

#include "Rendering/Renderer.h"

#include "AssetLoading/AssetManager.h"

#ifdef DEV_CONFIGURATION
#include <ScarletCoreEcs/Components/Generated/Register.generated.h>

#include <glfw/glfw3.h>

#include <ImGui/imgui_impl_glfw.h>
#include <ImGui/backends/imgui_impl_opengl3.h>

#include <Rendering/Framebuffer.h>
#endif // DEV_CONFIGURATION.

namespace Scarlet
{

Engine* Engine::mInstance = nullptr;

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

    DEBUG(Register::RegisterComponents());

    mAssetManager = new AssetManager();
    mAssetManager->LoadScarletAssets(Filepath{ FilepathDirectory::ENGINE, "EngineAssets/" });
    mAssetManager->LoadScarletAssets(Filepath{ FilepathDirectory::PROJECT, "" });

#ifdef DEV_CONFIGURATION
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    ImGui_ImplGlfw_InitForOpenGL(static_cast<GLFWwindow*>(Engine::Instance().GetMainWindow()->GetNativeWindow()), true);
    ImGui_ImplOpenGL3_Init("#version 330");
#endif // DEV_CONFIGURATION.

    mRunning = true;
    SCARLET_INFO("Engine Initialised!");
}

void Engine::Destroy() const noexcept
{
    SCARLET_ASSERT(!mRunning && "Trying to destroy engine whilst Engine::Run is running.");

#ifdef DEV_CONFIGURATION
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
#endif // DEV_CONFIGURATION.

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

#ifdef DEV_CONFIGURATION
        Framebuffer::Unbind();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        if (mEndRenderEvent) mEndRenderEvent();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupCurrentContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupCurrentContext);
        }
#endif // DEV_CONFIGURATION.

        mMainWindow->Update();

        ScarlEnt::Registry::Instance().PostUpdate();
    }
}

void Engine::RegisterEngineSystems(WeakHandle<ScarlEnt::Scene> scene)
{
    // Rendering System.
    scene->RegisterSystem<Component::Transform, Component::Mesh>([&](Component::Transform& transform, Component::Mesh& mesh) {
        auto meshAsset     = mAssetManager->GetAsset(mesh.mesh.assetType    , Ulid{ mesh.mesh.assetUlid     });
        auto materialAsset = mAssetManager->GetAsset(mesh.material.assetType, Ulid{ mesh.material.assetUlid });
        Renderer::Instance().AddRenderCommand(materialAsset, meshAsset,
            Math::TransformAsMatrix(transform.translation,
                Math::Trig::RotationMatrix(transform.rotation.z, transform.rotation.x, transform.rotation.y), transform.scale));
        });
}

#ifdef DEV_CONFIGURATION
void* Engine::GetImGuiContext()
{
    return ImGui::GetCurrentContext();
}
#endif // DEV_CONFIGURATION.

void Engine::OnEvent(Event& event)
{
    EventDispatcher dispatcher(event);
    dispatcher.Dispatch<WindowClosedEvent>([&](const WindowClosedEvent&) { mRunning = false; return true; });

    InputManager::OnEvent(event);
}

} // Namespace Scarlet.

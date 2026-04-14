#include "ScarletEnginePch.h"
#include "Engine.h"

#include <ScarletMath/Trig.h>

#include <ScarlEnt/Scene.h>

#include <ScarletCoreEcs/Components/Transform.h>
#include <ScarletCoreEcs/Components/StaticMesh.h>
#include <ScarletCoreEcs/Components/SphereCollider.h>

#include "Events/Event.h"
#include "Events/ApplicationEvents.h"

#include "Input/InputManager.h"

#include "Window/WindowManager.h"

#include "Rendering/Renderer.h"

#include "AssetLoading/AssetManager.h"

#include "Time.h"

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
#ifdef DEV_CONFIGURATION
    windowProperties.resizable = true;
    windowProperties.width     = 800;
    windowProperties.height    = 500;
#endif // DEV_CONFIGURATION.
    mMainWindow = WindowManager::CreateWindowInternal("Scarlet Editor", std::move(windowProperties));

    ScarlEnt::Registry::Init();

    Renderer::InitApi();

    DEBUG(Register::RegisterComponents());

    LoadScarletAssets();

    Time::Init();

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

    DEBUG(Register::UnregisterComponents());

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

        WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().GetActiveScene();
#ifdef DEV_CONFIGURATION
        if (scene.IsValid()) [[likely]]
#endif // ^^^ DEV_CONFIGURATION. ^^^
        {
            scene->Update();

            if (Time::Tick())
            {
                scene->FixedUpdate();

                ApplySpherePhysics();
            }
        }

#ifdef DEV_CONFIGURATION
        if (mBeginRenderEvent) mBeginRenderEvent();
#endif // ^^^ DEV_CONFIGURATION. ^^^

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
#ifdef DEV_CONFIGURATION
        if (scene.IsValid()) [[likely]]
#endif // DEV_CONFIGURATION.
            scene->PostUpdate();
        ScarlEnt::Registry::Instance().PostUpdate();

        DEBUG(if (mEditorPostUpdateEvent) mEditorPostUpdateEvent());
    }
}

void Engine::RegisterEngineSystems(WeakHandle<ScarlEnt::Scene> scene)
{
    // Rendering System.
    scene->RegisterSystem<Component::Transform, Component::StaticMesh>([&](Component::Transform& transform, Component::StaticMesh& mesh) {
        const WeakHandle<Resource::ILazyLoadAsset> meshAsset     = mAssetManager->GetAsset(mesh.mesh.assetType    , Ulid{ mesh.mesh.assetUlid     });
        const WeakHandle<Resource::ILazyLoadAsset> materialAsset = mAssetManager->GetAsset(mesh.material.assetType, Ulid{ mesh.material.assetUlid });
        Renderer::Instance().AddRenderCommand(materialAsset, meshAsset,
            Math::TransformAsMatrix(transform.translation,
                Math::Trig::RotationMatrix(transform.rotation.z, transform.rotation.x, transform.rotation.y), transform.scale));
    });

#ifdef DEV_CONFIGURATION
    scene->RegisterSystem<Component::Transform, Component::SphereCollider>([&](Component::Transform& transform, Component::SphereCollider& sphere) {
        const WeakHandle<Resource::ILazyLoadAsset> meshAsset     = mAssetManager->GetAsset(AssetType::MESH, Ulid{ 4 });
        const WeakHandle<Resource::ILazyLoadAsset> materialAsset = mAssetManager->GetAsset(AssetType::MATERIAL, AssetManager::INVALID_ULID);
        Renderer::Instance().AddRenderCommand(materialAsset, meshAsset,
            Math::TransformAsMatrix(transform.translation,
                Math::Trig::RotationMatrix(transform.rotation.z, transform.rotation.x, transform.rotation.y),
                Math::Vec3{ sphere.radius * 2.0f * transform.scale.x }));
    });
#endif // DEV_CONFIGURATION.
}

void Engine::LoadScarletAssets()
{
    SCARLET_ASSERT(!mAssetManager && "Trying to load in Scarlet Assets when there are already loaded assets.");

    mAssetManager = new AssetManager();

    mAssetManager->LoadScarletAssets(Filepath{ FilepathDirectory::ENGINE , "EngineAssets/" });

    if (!Filepath::GetProjectDirectory().empty())
    {
        mAssetManager->LoadScarletAssets(Filepath{ FilepathDirectory::PROJECT, "" });
    }
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

// Todo Christian: We should have a proper physics manager and physics system instead of just simply sphere collision. This ideally should be in its own project as well - not in the engine.cpp.
void Engine::ApplySpherePhysics()
{
    WeakHandle<ScarlEnt::Scene> currentScene = ScarlEnt::Registry::Instance().GetActiveScene();

    if (!currentScene.IsValid()) [[unlikely]]
    {
        return;
    }

    auto sphereEntities = currentScene->GetComponents<Component::Transform, Component::SphereCollider>();

    for (size_t i{ 0 }; i < sphereEntities.size(); ++i)
    {
        for (size_t j{ 0 }; j < sphereEntities.size(); ++j)
        {
            if (i == j)
            {
                continue;
            }

            Component::Transform& transformI   = std::get<Component::Transform&>(sphereEntities[i]);
            Component::Transform& transformJ   = std::get<Component::Transform&>(sphereEntities[j]);

            Component::SphereCollider& sphereI = std::get<Component::SphereCollider&>(sphereEntities[i]);
            Component::SphereCollider& sphereJ = std::get<Component::SphereCollider&>(sphereEntities[j]);

            const Math::Vec3 directionVector = transformJ.translation - transformI.translation;
            const float distSquared = Math::MagnitudeSquared(directionVector);

            const float radiusI = sphereI.radius * transformI.scale.x;
            const float radiusJ = sphereJ.radius * transformJ.scale.x;
            const float minDistanceSquared = static_cast<float>(std::pow(radiusI + radiusJ, 2));

            if (distSquared >= minDistanceSquared)
            {
                continue;
            }

            const float dist    = Math::Sqrt(distSquared);
            const float minDist = Math::Sqrt(minDistanceSquared);
            const float halfDifference = (minDist - dist) * 0.5f;

            const Math::Vec3 movement = directionVector / dist * halfDifference;
            transformJ.translation += movement;
            transformI.translation -= movement;

            if (sphereI.onCollisionCallback)
            {
                sphereI.onCollisionCallback(sphereJ);
            }
            if (sphereJ.onCollisionCallback)
            {
                sphereJ.onCollisionCallback(sphereI);
            }
        }
    }
}

} // Namespace Scarlet.

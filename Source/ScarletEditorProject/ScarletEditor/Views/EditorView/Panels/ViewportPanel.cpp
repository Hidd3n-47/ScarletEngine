#include "ScarletEditorPch.h"
#include "ViewportPanel.h"

#ifdef DEV_CONFIGURATION

#include <glew/glew.h>

#include <ScarlEnt/Scene.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>
#include <ScarletEngine/AssetLoading/AssetManager.h>

#include <ScarletEngine/Core/Input/KeyCodes.h>
#include <ScarletEngine/Core/Input/InputManager.h>

#include <ScarletEngine/Rendering/Framebuffer.h>

#include "Core/EditorManager.h"
#include "ScarletCoreEcs/Components/StaticMesh.h"
#include "ScarletCoreEcs/Components/Transform.h"
#include "ScarletCoreEcs/Components/EditorInfo.h"
#include "ScarletCoreEcs/Components/Camera.h"
#include "ScarletMath/BoundingBox.h"
#include "ScarletMath/Ray.h"
#include "ScarletMath/Trig.h"
#include "Views/EditorView/View/EditorView.h"

namespace Scarlet::Editor
{

ViewportPanel::ViewportPanel(IView* view)
    : Panel{ view, {.title = "Viewport" } }
{
    Engine& engineInstance = Engine::Instance();

    WeakHandle<Window> mainWindow = Engine::Instance().GetMainWindow();
    mViewportWidth  = mainWindow->GetWidth();
    mViewportHeight = mainWindow->GetHeight();

    mLastFrameWidth  = mViewportWidth;
    mLastFrameHeight = mViewportHeight;

    engineInstance.SetBeginRenderEvent([&] { BeginRender(); });
    mFramebuffer = new Framebuffer(mViewportWidth, mViewportHeight);
}

ViewportPanel::~ViewportPanel()
{
    delete mFramebuffer;
    Engine::Instance().SetBeginRenderEvent(nullptr);
}

void ViewportPanel::Render()
{
    const ImVec2 viewportSize = ImGui::GetWindowSize();

    constexpr float toolbarHeight = 30.0f;

    ImGui::BeginChild("ViewportToolbar", ImVec2(viewportSize.x, toolbarHeight), false, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    if (ImGui::Button("Translate")) {}
    ImGui::SameLine();
    if (ImGui::Button("Rotate")) {}
    ImGui::SameLine();
    if (ImGui::Button("Scale")) {}

    AssetManager& assetManager = Engine::Instance().GetAssetManager();
    ResourceManager<Resource::Texture>& textureManager = Renderer::Instance().GetTextureManager();

    const uint32 playIconId = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::PLAY_ICON_ULID)->GetRuntimeId())->GetId();
    const uint32 stopIconId = textureManager.GetResource(assetManager.GetAsset(AssetType::TEXTURE, EditorManager::STOP_ICON_ULID)->GetRuntimeId())->GetId();

    constexpr float iconWidth = 20.0f;

    ImGui::SameLine(viewportSize.x * 0.5f - 75.0f);
    if (ImGui::ImageButton("Play", playIconId, ImVec2{ iconWidth, iconWidth }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }) && !mGameSimulating)
    {
        mGameSimulating = true;
        WeakHandle<ScarlEnt::Scene> scene = EditorManager::Instance().GetGameScene();

        scene->RemoveAllSystems();
        scene->Init();
    }

    ImGui::SameLine(viewportSize.x * 0.5f - 25.0f);
    if (ImGui::ImageButton("Stop", stopIconId, ImVec2{ iconWidth, iconWidth }, ImVec2{ 0.0f, 1.0f }, ImVec2{ 1.0f, 0.0f }) && mGameSimulating)
    {
        EditorManager& instance           = EditorManager::Instance();
        WeakHandle<ScarlEnt::Scene> scene = instance.GetGameScene();

        ScarlEnt::Registry::Instance().RenameScene(scene, "Reloading...");

        EditorManager::Instance().OpenScene(EditorManager::Instance().GetCurrentScenePath());

        mGameSimulating = false;
    }

    ImGui::SameLine(viewportSize.x - 150);
    if (ImGui::Button("Hot Reload DLL"))
    {
        Engine::Instance().ReloadGameDll();
    }

    ImGui::EndChild();

    const ImVec2 remainingSize = ImVec2(viewportSize.x, viewportSize.y - toolbarHeight);

    const uint32 width  = static_cast<uint32>(remainingSize.x);
    const uint32 height = static_cast<uint32>(remainingSize.y);

    const bool resizing = mLastFrameWidth != width || mLastFrameHeight != height;

    mLastFrameWidth  = width;
    mLastFrameHeight = height;

    if (!resizing)
    {
        ImGui::Image(mFramebuffer->GetColorAttachmentId(), ImGui::GetContentRegionAvail(), ImVec2{ 0, 1 }, ImVec2{ 1, 0 });

        if (width != mViewportWidth || height != mViewportHeight)
        {
            mFramebuffer->Resize(width, height);

            mViewportWidth = width;
            mViewportHeight = height;
        }
    }
       
    ViewportSelection();
}

void ViewportPanel::BeginRender() const
{
    mFramebuffer->Bind();
    glViewport(0, 0, static_cast<int>(mViewportWidth), static_cast<int>(mViewportHeight));
}

void ViewportPanel::ViewportSelection() const
{
    if (!InputManager::IsKeyPressed(KeyCode::MOUSE_BUTTON_1) || !mIsHovered)
    {
        return;
    }

    SelectionManager& selectionManager = reinterpret_cast<EditorView*>(mView)->GetSelectionManager();

    // If the user has pressed Escape, reset the selection.
    if (InputManager::IsKeyPressed(KeyCode::KEY_ESCAPE))
    {
        selectionManager.SetSelectedEntity(nullptr);
        return;
    }

    const ImVec2 mouse    = ImGui::GetMousePos();
    const ImVec2 imageMin = ImGui::GetItemRectMin();
    const ImVec2 size     = ImGui::GetItemRectSize();

    const ImVec2 local{ mouse.x - imageMin.x, mouse.y - imageMin.y };

    const float normalisedX = (2.0f * local.x) / size.x - 1.0f;
    const float normalisedY = 1.0f - (2.0f * local.y) / size.y;

    Math::Vec4 farPoint{ normalisedX, normalisedY, 1.0f, 1.0f };

    Component::Camera& cam     = EditorManager::Instance().GetViewportCamera();
    Component::Transform& camT = EditorManager::Instance().GetViewportCameraTransform();

    Math::Mat4 invViewProj = Math::Inverse(cam.projectionMatrix * cam.viewMatrix);

    Math::Vec4 farWorld = invViewProj * farPoint;
    farWorld /= farWorld.w;

    Math::Vec3 rayDir = Math::Normalize(Math::Vec3{ farWorld } - camT.translation);

    Math::Ray ray{ camT.translation, rayDir };

    ScarlEnt::IEntityHandle* newSelected = nullptr;

    float closestT = std::numeric_limits<float>::max();
    for (ScarlEnt::IEntityHandle* handle : EditorManager::Instance().GetGameScene()->GetMutableEntityHandles())
    {
        auto* entity = reinterpret_cast<ScarlEnt::MutableEntityHandle*>(handle);

        Math::BoundingBox box;

        // Can only viewport select if the item has a static mesh.
        if (!entity->HasComponent<Component::StaticMesh>())
        {
            continue;
        }

        const Component::StaticMesh& mesh = entity->GetComponent<Component::StaticMesh>();

        // Get the bounding box from the mesh.
        WeakHandle<Resource::ILazyLoadAsset> lazyAsset = Engine::Instance().GetAssetManager().GetAsset(AssetType::MESH, Ulid{ mesh.mesh.assetUlid });
        const WeakHandle<Resource::Mesh> meshInfo      = Renderer::Instance().GetMeshManager().GetResource(lazyAsset->GetRuntimeId());

        Math::Vec3 lower{ 0.0f };
        Math::Vec3 upper{ 0.0f };
        meshInfo->GetBounds(lower, upper);

        const Component::Transform& transform = entity->GetComponent<Component::Transform>();
        box.SetTranslation(transform.translation + (lower + upper) * 0.5f);

        lower *= transform.scale;
        upper *= transform.scale;
        box.SetMinimumBound(lower);
        box.SetMaximumBound(upper);

        box.SetRotation(transform.rotation);

        if (const float t = ray.IntersectsWithBoundingBox(box); t > 0.0f && t < closestT)
        {
            closestT = t;
            newSelected = handle;
        }
    }

    selectionManager.SetSelectedEntity(newSelected);
}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

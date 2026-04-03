#include "ScarletEditorPch.h"
#include "ViewportPanel.h"

#ifdef DEV_CONFIGURATION

#include <glew/glew.h>

#include <ImGuizmo/ImGuizmo.h>

#include <ScarletMath/Ray.h>

#include <ScarlEnt/Scene.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>
#include <ScarletEngine/AssetLoading/AssetManager.h>

#include <ScarletEngine/Core/Input/KeyCodes.h>
#include <ScarletEngine/Core/Input/InputManager.h>

#include <ScarletEngine/Rendering/Framebuffer.h>

#include <ScarletCoreEcs/Components/Camera.h>
#include <ScarletCoreEcs/Components/Transform.h>
#include <ScarletCoreEcs/Components/StaticMesh.h>

#include "Core/EditorManager.h"
#include "Views/EditorView/View/EditorView.h"

namespace Scarlet::Editor
{

namespace
{
constexpr float VIEWPORT_TOOLBAR_HEIGHT = 30.0f;
constexpr ImVec4 BLUE_COLOR{ 0.0f, 0.43f, 0.89f, 1.0f };
} // Anonymous Namespace.


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
    SelectionManager& selectionManager = reinterpret_cast<EditorView*>(mView)->GetSelectionManager();

    const ImVec2 viewportSize  = ImGui::GetWindowSize();

    RenderViewportToolbar(selectionManager);

    const ImVec2 remainingSize = ImVec2(viewportSize.x, viewportSize.y - VIEWPORT_TOOLBAR_HEIGHT);

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

            mViewportWidth  = width;
            mViewportHeight = height;
        }
    }

    ViewportSelection(selectionManager);
}

void ViewportPanel::RenderViewportToolbar(SelectionManager& selectionManager)
{
    const ImVec2 viewportSize = ImGui::GetWindowSize();

    ImGui::BeginChild("ViewportToolbar", ImVec2{ viewportSize.x, VIEWPORT_TOOLBAR_HEIGHT }, 0, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

    ManipulatorScene& manipulatorScene = selectionManager.GetManipulatorScene();

    RenderToolbarTransformOperation(manipulatorScene, "Translate", ManipulatorOperation::TRANSLATE, KeyCode::KEY_W);
    ImGui::SameLine();
    RenderToolbarTransformOperation(manipulatorScene, "Rotate", ManipulatorOperation::ROTATE, KeyCode::KEY_E);
    ImGui::SameLine();
    RenderToolbarTransformOperation(manipulatorScene, "Scale", ManipulatorOperation::SCALE, KeyCode::KEY_R);

    RenderSnappingInterface(manipulatorScene);

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
        const std::string renamedSceneName{ "Reloading..." };

        const WeakHandle<ScarlEnt::Scene> scene = EditorManager::Instance().GetGameScene();
        const std::string activeSceneFriendlyName{ scene->GetFriendlyName() };

        Engine::Instance().ReloadGame();

        EditorManager::Instance().OpenScene(EditorManager::Instance().GetCurrentScenePath());

        mGameSimulating = false;
    }

    ImGui::SameLine(viewportSize.x - 250);
    if (ImGui::Button("Generate"))
    {
        const std::string enginePath  = Filepath{ FilepathDirectory::ENGINE , "" }.GetAbsolutePath();
        const std::string projectPath = Filepath{ FilepathDirectory::PROJECT, "" }.GetAbsolutePath();

        system(std::format("python {}/Build/GenerateRtti.py {} {}", enginePath, enginePath, projectPath).c_str());
        system(std::format("{}/Deps/Premake/premake5.exe --file=\"{}/premake5.lua\" vs2026", enginePath, projectPath).c_str());
    }

    ImGui::SameLine(viewportSize.x - 150);
    if (ImGui::Button("Hot Reload DLL"))
    {
        EditorManager::Instance().HotReloadGameDll();
    }

    ImGui::EndChild();
}

void ViewportPanel::RenderSnappingInterface(ManipulatorScene& manipulatorScene)
{
    const bool  isSnapping     = manipulatorScene.GetIsSnapping();
    const float snappingAmount = manipulatorScene.GetSnappingAmount();

    ImGui::SameLine(210.0f);

    if (isSnapping)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, BLUE_COLOR);
    }

    if (ImGui::Button("Snapping"))
    {
        manipulatorScene.SetIsSnapping(!isSnapping);
    }

    if (isSnapping)
    {
        ImGui::PopStyleColor();
    }

    constexpr int maxAmountOfDigits = 4;

    char amountCharArray[maxAmountOfDigits] = {};

    const std::string snappingAmountAsString = std::format("{:g}", snappingAmount);
    strcpy_s(amountCharArray, snappingAmountAsString.c_str());
    amountCharArray[maxAmountOfDigits - 1] = '\0';

    ImGui::PushItemWidth(50.0f);

    ImGui::SameLine();
    if (ImGui::InputText("##SnappingAmount", amountCharArray, maxAmountOfDigits, ImGuiInputTextFlags_EnterReturnsTrue))
    {
        manipulatorScene.SetSnappingAmount(std::stof(std::string{ amountCharArray }));
    }
}

void ViewportPanel::RenderToolbarTransformOperation(ManipulatorScene& manipulatorScene, const char* buttonLabel, const ManipulatorOperation buttonOperation, const uint16 shortcut)
{
    const bool enabledOperation = manipulatorScene.GetManipulatorOperation() == buttonOperation;

    if (enabledOperation)
    {
        ImGui::PushStyleColor(ImGuiCol_Button, BLUE_COLOR);
    }

    const bool shortcutPressedWhilstNotFlying = InputManager::IsKeyPressed(shortcut) && !InputManager::IsKeyDown(KeyCode::MOUSE_BUTTON_2);
    if (ImGui::Button(buttonLabel) || shortcutPressedWhilstNotFlying)
    {
        manipulatorScene.SetManipulatorOperation(buttonOperation);
    }

    if (enabledOperation)
    {
        ImGui::PopStyleColor();
    }
}

void ViewportPanel::BeginRender() const
{
    mFramebuffer->Bind();
    glViewport(0, 0, static_cast<int>(mViewportWidth), static_cast<int>(mViewportHeight));
}

void ViewportPanel::ViewportSelection(SelectionManager& selectionManager) const
{
    const ImVec2 imagePosition  = ImGui::GetItemRectMin();
    const ImVec2 imageDrawnSize = ImGui::GetItemRectSize();

    ImGuizmo::BeginFrame();
    ImGuizmo::SetDrawlist();
    ImGuizmo::SetOrthographic(false);
    ImGuizmo::AllowAxisFlip(false);

    ImGuizmo::SetRect(imagePosition.x, imagePosition.y, imageDrawnSize.x, imageDrawnSize.y);

    // If the manipulation is still occuring, early out as selection should not be altered mid transform.
    if (selectionManager.ManipulatingSelected())
    {
        return;
    }

    if (!InputManager::IsKeyPressed(KeyCode::MOUSE_BUTTON_1) || !mIsHovered)
    {
        return;
    }

    // If the user has pressed Escape, reset the selection.
    if (InputManager::IsKeyPressed(KeyCode::KEY_ESCAPE))
    {
        selectionManager.SetSelectedEntity(nullptr);
        return;
    }

    const ImVec2 mouse    = ImGui::GetMousePos();
    const ImVec2 imageMin = ImGui::GetItemRectMin();

    const ImVec2 local{ mouse.x - imageMin.x, mouse.y - imageMin.y };

    const float normalisedX = (2.0f * local.x) / imageDrawnSize.x - 1.0f;
    const float normalisedY = 1.0f - (2.0f * local.y) / imageDrawnSize.y;

    Math::Vec4 farPoint{ normalisedX, normalisedY, 1.0f, 1.0f };

    Component::Camera& cam     = EditorManager::Instance().GetViewportCamera();
    Component::Transform& camT = EditorManager::Instance().GetViewportCameraTransform();

    const Math::Mat4 invViewProj = Math::Inverse(cam.projectionMatrix * cam.viewMatrix);

    Math::Vec4 farWorld = invViewProj * farPoint;
    farWorld /= farWorld.w;

    const Math::Vec3 rayDir = Math::Normalize(Math::Vec3{ farWorld } - camT.translation);

    const Math::Ray ray{ camT.translation, rayDir };

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

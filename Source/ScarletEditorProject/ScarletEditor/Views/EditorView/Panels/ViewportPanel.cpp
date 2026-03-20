#include "ScarletEditorPch.h"
#include "ViewportPanel.h"

#ifdef DEV_CONFIGURATION

#include <glew/glew.h>

#include <ScarlEnt/Scene.h>

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>
#include <ScarletEngine/AssetLoading/AssetManager.h>

#include <ScarletEngine/Rendering/Framebuffer.h>

#include "Core/EditorManager.h"

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
}

void ViewportPanel::BeginRender() const
{
    mFramebuffer->Bind();
    glViewport(0, 0, static_cast<int>(mViewportWidth), static_cast<int>(mViewportHeight));

}

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

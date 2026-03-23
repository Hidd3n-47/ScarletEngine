#include "ScarletEditorPch.h"
#include "Panel.h"

namespace Scarlet::Editor
{

void Panel::RenderUi()
{
    const char* title = mProperties.title.data();
    ImGuiWindowFlags flags = 0;

    if (!mProperties.moveable)
    {
        flags |= ImGuiWindowFlags_NoMove;

        ImGui::SetNextWindowSize(ImVec2{ mProperties.width, mProperties.height });
    }

    if (!mProperties.resizable)
    {
        flags |= ImGuiWindowFlags_NoResize;

        const ImVec2 center{ ImGui::GetMainViewport()->GetCenter() };
        const ImVec2 size{ ImGui::GetIO().DisplaySize };
        const ImVec2 topLeft{ center.x - size.x * 0.5f + mProperties.positionX, center.y - size.y * 0.5f + mProperties.positionY };
        ImGui::SetNextWindowPos(topLeft);
    }

    flags |= mProperties.titleBar ? 0 : ImGuiWindowFlags_NoTitleBar;

    ImGui::Begin(title, nullptr, flags);

    RenderContextMenu();

    mIsFocused = ImGui::IsWindowFocused();
    mIsHovered = ImGui::IsWindowHovered();

    Render();

    ImGui::End();
}

} // Namespace Scarlet::Editor
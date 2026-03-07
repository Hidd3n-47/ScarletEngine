#include "Panel.h"

#include "imgui/imgui.h"

namespace Scarlet::Editor
{

void Panel::RenderUi()
{
    const char* title = mProperties.title.data();
    ImGui::Begin(title);

    RenderContextMenu();

    mIsFocused = ImGui::IsWindowFocused();
    mIsHovered = ImGui::IsWindowHovered();

    Render();

    ImGui::End();
}

} // Namespace Scarlet::Editor
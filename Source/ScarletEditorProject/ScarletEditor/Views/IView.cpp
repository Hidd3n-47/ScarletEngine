#include "ScarletEditorPch.h"
#include "IView.h"

namespace Scarlet::Editor
{

void IView::Render()
{
    constexpr ImGuiWindowFlags windowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking
        | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse   | ImGuiWindowFlags_NoResize
        | ImGuiWindowFlags_NoMove     | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

    const ImGuiViewport* viewport = ImGui::GetMainViewport();

    ImGui::SetNextWindowPos(viewport->WorkPos);
    ImGui::SetNextWindowSize(viewport->WorkSize);
    ImGui::SetNextWindowViewport(viewport->ID);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

    ImGui::Begin("DockSpace", nullptr, windowFlags);

    ImGui::PopStyleVar(3);

    const ImGuiIO& io = ImGui::GetIO();
    if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
    {
        ImGui::DockSpace(ImGui::GetID("DockSpace"), ImVec2{ 0.0f, 0.0f }, ImGuiDockNodeFlags_None);
    }

    RenderMenuBar();

    ImGui::End(); // DockSpace.

    for (const auto& panel : mViewPanels | std::views::values)
    {
        panel->RenderUi();
    }
}

} // Namespace Scarlet::Editor.
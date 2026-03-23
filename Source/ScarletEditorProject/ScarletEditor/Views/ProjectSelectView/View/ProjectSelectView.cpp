#include "ScarletEditorPch.h"
#include "ProjectSelectView.h"

#ifdef DEV_CONFIGURATION

#include <ScarletEngine/Core/Engine.h>
#include <ScarletEngine/Core/Window/Window.h>

#include "Views/ProjectSelectView/Panels/NewProjectPanel.h"
#include "Views/ProjectSelectView/Panels/RecentProjectsPanel.h"


namespace Scarlet::Editor
{

ProjectSelectView::ProjectSelectView()
{
    WeakHandle<Window> window = Engine::Instance().GetMainWindow();
    window->SetTitle("Scarlet - Project Select");
    window->SetPosition(-1, -1);

    AddPanel<NewProjectPanel>();
    AddPanel<RecentProjectsPanel>();
}

} // Scarlet::Editor.

#endif // DEV_CONFIGURATION.
#pragma once

#ifdef DEV_CONFIGURATION

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class ScenePanel: A panel used to show the recently opened Scarlet Projects.
 */
class RecentProjectsPanel final : public Panel
{
public:
    explicit RecentProjectsPanel(IView* view);
    ~RecentProjectsPanel() override;

    RecentProjectsPanel(const RecentProjectsPanel&)            = delete;
    RecentProjectsPanel(RecentProjectsPanel&&)                 = delete;
    RecentProjectsPanel& operator=(RecentProjectsPanel&&)      = delete;
    RecentProjectsPanel& operator=(const RecentProjectsPanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;

    inline void AddRecentProject(std::string projectPath) { mRecentProjects.emplace_back(std::move(projectPath)); }
private:
    vector<std::string> mRecentProjects;

    static std::string GetRecentProjectFilepath();
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

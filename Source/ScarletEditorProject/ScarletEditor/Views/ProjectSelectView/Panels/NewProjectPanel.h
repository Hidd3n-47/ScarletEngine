#pragma once

#ifdef DEV_CONFIGURATION

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class NewProjectPanel: A panel used to create new projects.
 */
class NewProjectPanel final : public Panel
{
public:
    explicit NewProjectPanel(IView* view);
    ~NewProjectPanel() override = default;

    NewProjectPanel(const NewProjectPanel&)            = delete;
    NewProjectPanel(NewProjectPanel&&)                 = delete;
    NewProjectPanel& operator=(NewProjectPanel&&)      = delete;
    NewProjectPanel& operator=(const NewProjectPanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;
private:
    std::string mNewProjPath;
    std::string mNewProjName;
    std::string mNewSceneName{ SCENE_NAME_DEFAULT };
    bool        mProjectNameContainsSpace{ false };
    bool        mSceneNameContainsSpace  { false };

    void RenderProjectName();
    void RenderSceneName();
    void RenderFolderPicker();

    void Create();

    inline static const std::string SCENE_NAME_DEFAULT{ "StartingScene" };
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

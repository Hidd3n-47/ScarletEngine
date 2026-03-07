#pragma once

#ifdef DEV_CONFIGURATION

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class ScenePanel: A panel used to show the currently active scene and all the entities in the scene.
 */
class ScenePanel final : public Panel
{
public:
    explicit inline ScenePanel(IView* view) : Panel{ view, {.title = "Scene" } } { /* Empty */ }
    ~ScenePanel() override = default;

    ScenePanel(const ScenePanel&)            = delete;
    ScenePanel(ScenePanel&&)                 = delete;
    ScenePanel& operator=(ScenePanel&&)      = delete;
    ScenePanel& operator=(const ScenePanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

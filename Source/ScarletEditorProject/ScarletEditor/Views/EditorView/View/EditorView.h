#pragma once

#ifdef DEV_CONFIGURATION

#include "Views/IView.h"

#include "Core/SelectionManager.h"

namespace ScarlEnt
{
class Scene;
} // Namespace ScarlEnt.

namespace Scarlet::Editor
{

class Panel;

/**
 * @class EditorView: The main view and collection of panels for the main editor interface.
 */
class EditorView final : public IView
{
friend class EditorViewInputLayer;
public:
    EditorView();
    ~EditorView() override = default;

    EditorView(const EditorView&)               = delete;
    EditorView(EditorView&&)                    = delete;
    EditorView& operator=(EditorView&&)         = delete;
    EditorView& operator=(const EditorView&)    = delete;

    /**
     * @copydoc IView::RenderMenuBar
     */
    void RenderMenuBar() override;

    /** @brief Get the selection manager. */
    [[nodiscard]] inline SelectionManager& GetSelectionManager() { return mSelectionManager; }
private:
    SelectionManager mSelectionManager;

    bool mPropertiesPanel = true;
    bool mScenePanel      = true;
    bool mViewportPanel   = true;
    bool mConsolePanel    = true;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
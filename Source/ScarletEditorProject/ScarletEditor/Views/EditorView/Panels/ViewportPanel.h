#pragma once

#ifdef DEV_CONFIGURATION

#include "UI/Panel.h"

#include "Core/ManipulatorScene.h"

namespace Scarlet
{

class Framebuffer;

} // Namespace Scarlet.

namespace Scarlet::Editor
{

class SelectionManager;

/**
 * @class ViewportPanel: A panel to render the viewport. This is what the runtime scene would look like.
 */
class ViewportPanel final : public Panel
{
public:
    explicit ViewportPanel(IView* view);
    ~ViewportPanel() override;

    ViewportPanel(const ViewportPanel&)            = delete;
    ViewportPanel(ViewportPanel&&)                 = delete;
    ViewportPanel& operator=(ViewportPanel&&)      = delete;
    ViewportPanel& operator=(const ViewportPanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;
private:
    uint32 mLastFrameWidth, mLastFrameHeight;
    uint32 mViewportWidth , mViewportHeight;

    Framebuffer* mFramebuffer;

    bool mGameSimulating = false;

    // Todo Christian: Change this to a component so that this happens before the render.
    /**
     * @brief This function is called from the engine so that the framebuffer can be bound before the render begins.
     */
    void BeginRender() const;

    /**
     * @brief Render the toolbar above the viewport. This contains some buttons for things like current transform gizmo, DLL hot-reloading etc.
     * @param selectionManager The \ref SelectionManager of the editor to allow for setting the transform mode from UI.
     */
    void RenderViewportToolbar(SelectionManager& selectionManager);

    /**
     * @brief Render the toolbar transform button to allow for users to change transform mode via UI buttons.
     * @param manipulatorScene The \ref ManipulatorScene responsible for manipulating the selected entity(ies).
     * @param buttonLabel The label appearing on the button.
     * @param buttonOperation The \ref ManipulatorOperation the button is responsible for.
     * @param shortcut The \ref KeyCode used to change to this operation.
     */
    static void RenderToolbarTransformOperation(ManipulatorScene& manipulatorScene, const char* buttonLabel, const ManipulatorOperation buttonOperation, const uint16 shortcut);

    /**
     * @brief Handles viewport selection via ray-casting.
     */
    void ViewportSelection(SelectionManager& selectionManager) const;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
#pragma once

#ifdef DEV_CONFIGURATION

#include "UI/Panel.h"

namespace Scarlet
{

class Framebuffer;

} // Namespace Scarlet.

namespace Scarlet::Editor
{

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
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.
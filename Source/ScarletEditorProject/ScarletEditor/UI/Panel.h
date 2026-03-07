#pragma once

namespace Scarlet::Editor
{

struct PanelProperties
{
    std::string_view title = "NoTitle";
};

class IView;

/**
 * @class Panel: A panel is a window that can be docked into the main dock-space of the editor UI.
 */
class Panel
{
public:
    explicit inline Panel(IView* view, const PanelProperties& properties = PanelProperties()) : mView{ view }, mProperties{ properties } { /* Empty. */ }
    virtual ~Panel() = default;

    Panel(const Panel&)            = delete;
    Panel(Panel&&)                 = delete;
    Panel& operator=(Panel&&)      = delete;
    Panel& operator=(const Panel&) = delete;

    /**
     * @brief Render the UI of the panel.
     */
    void RenderUi();

    /**
     * @brief Get if the panel is focused.
     * @return \c true if the panel is focused, \c false otherwise.
     */
    [[nodiscard]] inline bool IsFocused() const { return mIsFocused; }
    /**
     * @brief Get if the panel is currently hovered over with the mouse.
     * @return \c true if the panel is currently hovered over with the mouse, \c otherwise false.
     */
    [[nodiscard]] inline bool IsHovered() const { return mIsHovered; }
protected:
    IView* mView;

    bool mIsFocused = false;
    bool mIsHovered = false;

    /**
     * @brief A method called when the right-mouse button is pressed to open the context menu.
     */
    virtual void RenderContextMenu() {}
private:
    PanelProperties mProperties;

    /**
     * @brief Internal render function where the render code of the panel is added.
     */
    virtual void Render() = 0;

};

} // Namespace Scarlet::Editor.
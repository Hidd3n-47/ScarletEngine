#pragma once

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class IView: A view is defined to be a collection of panels that can be viewed at the same time. \\n
 * The editor view is a good example of a view, contains multiple panels. Views can be used to have default configuration and sets of panels.
 */
class IView
{
public:
    IView()          = default;
    virtual ~IView() = default;

    IView(const IView&)            = delete;
    IView(IView&&)                 = delete;
    IView& operator=(IView&&)      = delete;
    IView& operator=(const IView&) = delete;
    /**
     * @brief A function used to render the menu bar at the top of the window (file, edit, window etc.).
     */
    virtual void RenderMenuBar() { }

    /**
     * @brief Render the view and all the added panels.
     */
    void Render();

    /**
     * @brief Add a panel to the view.
     * @tparam T The panel class being added to the view.
     */
    template <typename T>
    void AddPanel();

    /**
     * @brief Remove a panel from the view.
     * @tparam T The panel class being removed from the view.
     */
    template <typename T>
    void RemovePanel();

    /**
     * @brief Get a specific panel from the view.
     * @tparam T The panel class being requested.
     * @return A shared pointer to the panel that has been requested.
     */
    template <typename T>
    std::shared_ptr<Panel> GetPanel();
protected:
    unordered_map<std::string, std::shared_ptr<Panel>> mViewPanels;
};

/* ============================================================================================================================== */

template <typename T>
void IView::AddPanel()
{
    const char* id = typeid(T).name();
    const std::string typeName(id);

    if (mViewPanels.contains(typeName))
    {
        SCARLET_ERROR("Trying to add panel that is already present in current view.");
        return;
    }

    mViewPanels[typeName] = std::make_shared<T>(this);
}

template <typename T>
void IView::RemovePanel()
{
    const char* id = typeid(T).name();
    const std::string typeName(id);

    if (!mViewPanels.contains(typeName))
    {
        SCARLET_ERROR("Trying to remove panel that is is not present in current view.");
        return;
    }

    mViewPanels.erase(typeName);
}

template <typename T>
std::shared_ptr<Panel> IView::GetPanel()
{
    const char* id = typeid(T).name();
    const std::string typeName(id);

    if (!mViewPanels.contains(typeName))
    {
        SCARLET_ERROR("Trying to retrieve panel that is is not present in current view.");
        return std::shared_ptr<T>(nullptr);
    }

    return mViewPanels[typeName];
}

} // Namespace Scarlet::Editor.
#pragma once

#include "UI/Panel.h"

namespace Scarlet::Editor
{

/**
 * @class AssetBrowserPanel: The panel used to show the assets and directories.
 */
class AssetBrowserPanel final : public Panel
{
public:
    explicit AssetBrowserPanel(IView* view);
    ~AssetBrowserPanel() override = default;

    AssetBrowserPanel(const AssetBrowserPanel&)            = delete;
    AssetBrowserPanel(AssetBrowserPanel&&)                 = delete;
    AssetBrowserPanel& operator=(AssetBrowserPanel&&)      = delete;
    AssetBrowserPanel& operator=(const AssetBrowserPanel&) = delete;

    /** @copydoc Panel::Render */
    void Render() override;

    inline static constexpr uint32 MAX_FILEPATH_LENGTH{ 260 };
private:
    Filepath mCurrentDirectory;

    void RenderBackAndPathTextInput(const uint32 backIconId);
    static void RenderIconLabel(const std::string& label, const float iconWidth);
};

} // Namespace Scarlet::Editor.

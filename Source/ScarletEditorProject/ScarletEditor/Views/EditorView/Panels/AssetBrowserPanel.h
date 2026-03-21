#pragma once

#ifdef DEV_CONFIGURATION

#include <ScarletCore/AssetRef.h>

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

    AssetType   mAssetTypeToCreate = AssetType::ASSET_TYPE_COUNT;
    bool mCreateFile   = false;
    bool mCreateFolder = false;
    bool mContextOpen  = false;

    void RenderBackAndPathTextInput(const uint32 backIconId);
    static void RenderIconLabel(const std::string& label, const float iconWidth);

    void RenderContextMenu() override;

    void CreateAssetPopup();

    void CreateFolder();
    void CreateTexture();
    void CreateMaterial();
    void CreateMesh();

    void CreateMenuWithSelectingAssetFromBrowser(const AssetType type, const char* browserFilter);

    static constexpr float CREATE_ASSET_WIDTH  = 450.0f;
    static constexpr float CREATE_ASSET_HEIGHT = 250.0f;
};

} // Namespace Scarlet::Editor.

#endif // DEV_CONFIGURATION.

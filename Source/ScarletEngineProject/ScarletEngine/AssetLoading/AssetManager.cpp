#include "ScarletEnginePch.h"
#include "AssetManager.h"

#include <ranges>
#include <filesystem>

#include <ScarletCore/Xml/XmlSerializer.h>

namespace Scarlet
{

AssetManager::~AssetManager()
{
    for (const unordered_map<Ulid, Resource::ILazyLoadAsset*>& typeArray : mTypeArrayForUlidToAssets)
    {
        for (const Resource::ILazyLoadAsset*  asset : typeArray | std::views::values)
        {
            delete asset;
        }
    }
}

void AssetManager::LoadScarletAssets()
{
    const std::string engineAssetPath = "E:/Programming/ScarletEngine/EngineAssets/";

    for (const auto& entry : std::filesystem::recursive_directory_iterator(engineAssetPath))
    {
        if (entry.path().extension() == ".scarlet")
        {
            XmlDocument assetXml = XmlSerializer::Deserialize(entry.path().string());

            const AssetType type{ static_cast<AssetType>(std::stoi(assetXml.GetRootElement()->GetAttributeValue("AssetType"))) };
            const Ulid      ulid{ std::stoull(assetXml.GetRootElement()->GetAttributeValue("ulid")) };

            if (type == AssetType::MESH)
            {
                const std::string& filepath = engineAssetPath + assetXml.GetRootElement()->GetChildElements()[0].GetValue();
                (void)CreateAsset<Resource::Mesh>(AssetType::MESH, filepath, ulid);
            }
            else if (type == AssetType::TEXTURE)
            {
                const std::string& filepath = engineAssetPath + assetXml.GetRootElement()->GetChildElements()[0].GetValue();
                (void)CreateAsset<Resource::Texture>(AssetType::TEXTURE, filepath, ulid);
            }
            else if (type == AssetType::MATERIAL)
            {
                (void)CreateAsset<Resource::Material>(AssetType::MATERIAL, entry.path().string(), ulid);
            }
        }
    }
}

void AssetManager::RemoveAsset(WeakHandle<Resource::ILazyLoadAsset>& asset)
{
    const int assetType = static_cast<int>(asset->GetAssetType());
    const Ulid ulid{ asset->GetUlid() };

    delete mTypeArrayForUlidToAssets[assetType][ulid];
    mTypeArrayForUlidToAssets[assetType].erase(ulid);

    asset.Invalidate();
}

} // Namespace Scarlet.
#include "ScarletEnginePch.h"
#include "AssetManager.h"

#include <ranges>
#include <filesystem>

#include <ScarletCore/AssetRef.h>
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

void AssetManager::LoadScarletAssets(const Filepath& assetPath)
{
    const std::filesystem::path startingPath{ assetPath.GetAbsolutePath() };
    for (const auto& entry : std::filesystem::recursive_directory_iterator(startingPath))
    {
        if (entry.path().extension() == ".scarlet")
        {
            XmlDocument assetXml = XmlSerializer::Deserialize(entry.path().string());

            // Relative path to the directory walk, this would be subdirectories from the parameter asset path.
            const Filepath relativePath = assetPath / std::filesystem::relative(std::filesystem::absolute(entry.path()).remove_filename(), startingPath);

            const AssetType type{ static_cast<AssetType>(std::stoi(assetXml.GetRootElement()->GetAttributeValue("AssetType"))) };
            const Ulid      ulid{ std::stoull(assetXml.GetRootElement()->GetAttributeValue("ulid")) };

            if (type == AssetType::MESH)
            {
                const Filepath filepath = relativePath / assetXml.GetRootElement()->GetChildElements()[0].GetValue();
                (void)CreateAsset<Resource::Mesh>(AssetType::MESH, filepath, ulid);
            }
            else if (type == AssetType::TEXTURE)
            {
                const Filepath filepath = relativePath / assetXml.GetRootElement()->GetChildElements()[0].GetValue();
                (void)CreateAsset<Resource::Texture>(AssetType::TEXTURE, filepath, ulid);
            }
            else if (type == AssetType::MATERIAL)
            {
                const Filepath filepath = assetPath / std::filesystem::relative(entry.path(), startingPath);
                (void)CreateAsset<Resource::Material>(AssetType::MATERIAL, filepath, ulid);
            }
        }
    }
}

void AssetManager::LoadAsset(const AssetType type, const Filepath& filepath, const Ulid ulid)
{
    if (type == AssetType::MESH)
    {
        (void)CreateAsset<Resource::Mesh>(AssetType::MESH, filepath, ulid);
    }
    else if (type == AssetType::TEXTURE)
    {
        (void)CreateAsset<Resource::Texture>(AssetType::TEXTURE, filepath, ulid);
    }
    else if (type == AssetType::MATERIAL)
    {
        (void)CreateAsset<Resource::Material>(AssetType::MATERIAL, filepath, ulid);
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

std::string AssetManager::AssetTypeToString(const AssetType type)
{
    switch (type)
    {
    case AssetType::TEXTURE:
        return "Texture";
    case AssetType::MESH:
        return "Static Mesh";
    case AssetType::MATERIAL:
        return "Material";
    case AssetType::ASSET_TYPE_COUNT:
    default:
        SCARLET_WARN("Requesting asset type string of an unhandled case.");
        return "UNKNOWN_ASSET_STRING";
    }
}

} // Namespace Scarlet.
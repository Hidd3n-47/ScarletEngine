#include "ScarletEnginePch.h"
#include "Material.h"

#include <ScarletCore/Xml/XmlSerializer.h>

#include <ScarletReflect/ReflectType.h>

#include "Core/Engine.h"
#include "AssetLoading/AssetManager.h"


namespace Scarlet::Resource
{

Material::Material(const Filepath& filepath)
{
    const XmlDocument document = XmlSerializer::Deserialize(filepath.GetAbsolutePath());

    for (const XmlElement& element : document.GetRootElement()->GetChildElements())
    {
        if (const std::string& propertyName = element.GetTagName(); propertyName == "texture")
        {
            const Ulid textureUlid{ std::stoull(element.GetValue()) };
            texture = Engine::Instance().GetAssetManager().GetAsset(AssetType::TEXTURE, textureUlid);
        }
        else if (propertyName == "ambientColor")
        {
            ReflectType::SetValueFromString(ambientColor, element.GetValue());
        } 
        else if (propertyName == "diffuseColor")
        {
            ReflectType::SetValueFromString(diffuseColor, element.GetValue());
        }
    }
}

} // Namespace Scarlet::Resource.
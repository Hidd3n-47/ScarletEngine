#include "Components/EditorInfo.h"

#include <ScarlEnt/ComponentManager.h>

#include <ScarletReflect/ReflectType.h>

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void EditorInfo::GenerateProperties()
{
    mProperties.clear();
    mProperties["name"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::STRING,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<EditorInfo>(),
        [this] { return ReflectType::GetStringFromValue(this->name); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->name, stringValue); }
    };

    mProperties["isMutable"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::BOOL,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<EditorInfo>(),
        [this] { return ReflectType::GetStringFromValue(this->isMutable); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->isMutable, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

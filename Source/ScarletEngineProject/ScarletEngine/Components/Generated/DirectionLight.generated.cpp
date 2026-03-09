#include "ScarletEnginePch.h"
#include "Components/DirectionLight.h"

#include <ScarlEnt/ComponentManager.h>
#include "RTTI/ReflectType.h"

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void DirectionLight::GenerateProperties()
{
    mProperties.clear();

    mProperties["lightColor"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<DirectionLight>(),
        [this] { return ReflectType::GetStringFromValue(this->lightColor); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->lightColor, stringValue); }
    };

    mProperties["direction"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<DirectionLight>(),
        [this] { return ReflectType::GetStringFromValue(this->direction); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->direction, stringValue); }
    };

    mProperties["ambientIntensity"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<DirectionLight>(),
        [this] { return ReflectType::GetStringFromValue(this->ambientIntensity); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->ambientIntensity, stringValue); }
    };

    mProperties["diffuseIntensity"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<DirectionLight>(),
        [this] { return ReflectType::GetStringFromValue(this->diffuseIntensity); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->diffuseIntensity, stringValue); }
    };

    mProperties["rimPower"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<DirectionLight>(),
        [this] { return ReflectType::GetStringFromValue(this->rimPower); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->rimPower, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

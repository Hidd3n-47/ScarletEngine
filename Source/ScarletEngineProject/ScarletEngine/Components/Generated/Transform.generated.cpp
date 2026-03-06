#include "ScarletEnginePch.h"
#include "Components/Transform.h"

#include <ScarlEnt/ComponentManager.h>
#include "RTTI/ReflectType.h"

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void Transform::GenerateProperties()
{
    mProperties.clear();

    mProperties["position"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Transform>(),
        [this] { return ReflectType::GetStringFromValue(this->position); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->position, stringValue); }
    };

    mProperties["scale"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Transform>(),
        [this] { return ReflectType::GetStringFromValue(this->scale); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->scale, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

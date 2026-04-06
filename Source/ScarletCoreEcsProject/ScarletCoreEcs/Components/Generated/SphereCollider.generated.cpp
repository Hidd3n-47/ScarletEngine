#include "Components/SphereCollider.h"

#include <ScarlEnt/ComponentManager.h>

#include <ScarletReflect/ReflectType.h>

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void SphereCollider::GenerateProperties()
{
    mProperties.clear();
    mProperties["radius"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<SphereCollider>(),
        [this] { return ReflectType::GetStringFromValue(this->radius); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->radius, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

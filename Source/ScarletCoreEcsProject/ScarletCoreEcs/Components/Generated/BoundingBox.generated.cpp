#include "Components/BoundingBox.h"

#include <ScarlEnt/ComponentManager.h>

#include <ScarletReflect/ReflectType.h>

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void BoundingBox::GenerateProperties()
{
    mProperties.clear();
    mProperties["localMinimum"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<BoundingBox>(),
        [this] { return ReflectType::GetStringFromValue(this->localMinimum); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->localMinimum, stringValue); }
    };

    mProperties["localMaximum"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<BoundingBox>(),
        [this] { return ReflectType::GetStringFromValue(this->localMaximum); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->localMaximum, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

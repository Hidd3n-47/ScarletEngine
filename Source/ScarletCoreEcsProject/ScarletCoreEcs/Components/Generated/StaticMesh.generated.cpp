#include "Components/StaticMesh.h"

#include <ScarlEnt/ComponentManager.h>

#include <ScarletReflect/ReflectType.h>

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void StaticMesh::GenerateProperties()
{
    mProperties.clear();
    mProperties["mesh"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::ASSET,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<StaticMesh>(),
        [this] { return ReflectType::GetStringFromValue(this->mesh); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->mesh, stringValue); }
    };

    mProperties["material"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::ASSET,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<StaticMesh>(),
        [this] { return ReflectType::GetStringFromValue(this->material); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->material, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

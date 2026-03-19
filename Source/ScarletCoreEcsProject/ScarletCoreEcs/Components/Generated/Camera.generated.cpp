#include "Components/Camera.h"

#include <ScarlEnt/ComponentManager.h>

#include <ScarletReflect/ReflectType.h>

namespace Scarlet::Component
{

#ifdef DEV_CONFIGURATION

void Camera::GenerateProperties()
{
    mProperties.clear();
    mProperties["forwardVector"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->forwardVector); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->forwardVector, stringValue); }
    };

        mProperties["rightVector"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->rightVector); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->rightVector, stringValue); }
    };

        mProperties["upVector"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::VEC3,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->upVector); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->upVector, stringValue); }
    };

        mProperties["aspectRatio"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->aspectRatio); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->aspectRatio, stringValue); }
    };

        mProperties["nearPlane"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->nearPlane); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->nearPlane, stringValue); }
    };

        mProperties["farPlane"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->farPlane); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->farPlane, stringValue); }
    };

        mProperties["fov"] = ScarlEnt::Property {
        ScarlEnt::PropertyType::FLOAT,
        ScarlEnt::Registry::Instance().GetOrRegisterComponentId<Camera>(),
        [this] { return ReflectType::GetStringFromValue(this->fov); },
        [this](const std::string_view& stringValue) { ReflectType::SetValueFromString(this->fov, stringValue); }
    };
};

#endif // DEV_CONFIGURATION.

} // Namespace Scarlet::Component.

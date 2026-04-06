#include "Register.generated.h"

#ifdef DEV_CONFIGURATION

void Register::RegisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::BoundingBox>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Camera>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::DirectionLight>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::EditorInfo>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::SphereCollider>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::StaticMesh>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Transform>(registry);

}

void Register::UnregisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    UnregisterComponentType<Scarlet::Component::BoundingBox>(registry);
    UnregisterComponentType<Scarlet::Component::Camera>(registry);
    UnregisterComponentType<Scarlet::Component::DirectionLight>(registry);
    UnregisterComponentType<Scarlet::Component::EditorInfo>(registry);
    UnregisterComponentType<Scarlet::Component::SphereCollider>(registry);
    UnregisterComponentType<Scarlet::Component::StaticMesh>(registry);
    UnregisterComponentType<Scarlet::Component::Transform>(registry);

}

#endif // DEV_CONFIGURATION.

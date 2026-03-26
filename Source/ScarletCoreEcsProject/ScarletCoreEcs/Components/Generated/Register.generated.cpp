#include "Register.generated.h"

#ifdef DEV_CONFIGURATION

void Register::RegisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::BoundingBox>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Camera>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::DirectionLight>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::EditorInfo>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::StaticMesh>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Transform>(registry);

}

#endif // DEV_CONFIGURATION.

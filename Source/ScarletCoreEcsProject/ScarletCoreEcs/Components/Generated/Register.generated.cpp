#include "Register.generated.h"

#ifdef DEV_CONFIGURATION

void Register::RegisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Camera>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::DirectionLight>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Mesh>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Transform>(registry);

}

#endif // DEV_CONFIGURATION.

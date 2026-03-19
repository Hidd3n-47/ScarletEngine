#include "Register.generated.h"

void Register::RegisterComponents()
{
    ScarlEnt::Registry& registry = ScarlEnt::Registry::Instance();

    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Camera>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::DirectionLight>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Mesh>(registry);
    RegisterComponentTypeAndFunctionPointer<Scarlet::Component::Transform>(registry);
}

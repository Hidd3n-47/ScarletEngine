#pragma once
#include <ScarlEnt/Scene.h>

#include <ScarletEngine/Core/Engine.h>

#include <ScarletCoreEcs/Components/Transform.h>

class ScarletTestGameProject : public ScarlEnt::Scene
{
public:
    inline ScarletTestGameProject(const std::string_view friendlyName) : Scene(friendlyName) { }

    void Init() override final
    {
        Scarlet::Engine::Instance().RegisterEngineSystems(Scarlet::WeakHandle<Scene>{ this });

        RegisterSystem<Scarlet::Component::Transform>([](Scarlet::Component::Transform& t)
            {
                t.rotation.z += 0.1f;
                if (t.rotation.z >= 360.0f)
                {
                    t.rotation.z = 0.0f;
                }
            });
    }
};


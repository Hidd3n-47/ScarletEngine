#pragma once
#include <ScarlEnt/Scene.h>

#include <ScarletEngine/Core/Engine.h>

#include <ScarletCoreEcs/Components/Transform.h>

class StartingScene : public ScarlEnt::Scene
{
public:
    inline StartingScene(const std::string_view friendlyName) : Scene(friendlyName) { }

    inline void Init() override final
    {
        // Register the engine systems. Leave this for default engine systems such as rendering, physics etc..
        Scarlet::Engine::Instance().RegisterEngineSystems(Scarlet::WeakHandle<Scene>{ this });

        // Example System Registered to rotate Objects with transform in the z-axis.
        RegisterSystem<Scarlet::Component::Transform>([](Scarlet::Component::Transform& t) {
            t.rotation.z += 0.1f;
            if (t.rotation.z >= 360.0f)
            {
                t.rotation.z = 0.0f;
            }
        });
    }
};


#pragma once

#ifndef DEV_CONFIGURATION

#include <ScarletCore/Defines.h>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>
#include <ScarlEnt/MutableEntityHandle.h>

#include <ScarletCoreEcs/Components/Generated/Register.generated.h>

namespace Scarlet
{

inline void InitGameScene()
{
    WeakHandle<ScarlEnt::Scene> scene = ScarlEnt::Registry::Instance().GetScene("ScarletTestGameProject");
    ScarlEnt::Registry::Instance().SetActiveScene(scene);

    {
        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
        auto& Transform = ent.AddComponent<Component::Transform>();
        Transform.translation = Math::Vec3{ -4.050000,0.000000,-1.350000 };
        Transform.rotation = Math::Vec3{ 0.000000,0.000000,0.000000 };
        Transform.scale = Math::Vec3{ 1.000000,1.000000,1.000000 };
        auto& Mesh = ent.AddComponent<Component::Mesh>();
        Mesh.mesh = AssetRef{ 0,0 };
        Mesh.material = AssetRef{ 2,0 };
    }

    {
        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
        auto& Transform = ent.AddComponent<Component::Transform>();
        Transform.translation = Math::Vec3{ 3.700000,0.000000,0.000000 };
        Transform.rotation = Math::Vec3{ 0.000000,0.000000,0.000000 };
        Transform.scale = Math::Vec3{ 1.000000,1.000000,1.000000 };
        auto& Mesh = ent.AddComponent<Component::Mesh>();
        Mesh.mesh = AssetRef{ 0,26604845367616295 };
        Mesh.material = AssetRef{ 2,26949044722005844 };
    }

    {
        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
        auto& Transform = ent.AddComponent<Component::Transform>();
        Transform.translation = Math::Vec3{ -2.650000,-1.250000,2.500000 };
        Transform.rotation = Math::Vec3{ 0.900000,-24.150000,0.000000 };
        Transform.scale = Math::Vec3{ 1.000000,1.000000,1.000000 };
        auto& Mesh = ent.AddComponent<Component::Mesh>();
        Mesh.mesh = AssetRef{ 0,26605124924805398 };
        Mesh.material = AssetRef{ 2,26949044722005844 };
    }

    {
        ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
        auto& Transform = ent.AddComponent<Component::Transform>();
        Transform.translation = Math::Vec3{ 4.950000,0.000000,5.600000 };
        Transform.rotation = Math::Vec3{ 0.000000,0.000000,0.000000 };
        Transform.scale = Math::Vec3{ 20.000000,20.000000,20.000000 };
        auto& Mesh = ent.AddComponent<Component::Mesh>();
        Mesh.mesh = AssetRef{ 0,26558880580407306 };
        Mesh.material = AssetRef{ 2,26949044722005844 };
    }

    {
        static ScarlEnt::MutableEntityHandle ent = scene->AddMutableEntity();
        auto& Transform = ent.AddComponent<Component::Transform>();
        Transform.translation = Math::Vec3{ 1.300000,-16.400000,2.000000 };
        Transform.rotation = Math::Vec3{ 2.100000,0.000000,-3.600000 };
        Transform.scale = Math::Vec3{ 1.000000,1.000000,1.000000 };
        auto& Camera = ent.AddComponent<Component::Camera>();
        Camera.forwardVector = Math::Vec3{ -0.063000,0.997000,-0.037000 };
        Camera.rightVector = Math::Vec3{ 0.998000,0.063000,0.000000 };
        Camera.nearPlane = float{ 0.100000 };
        Camera.upVector = Math::Vec3{ -0.002000,0.037000,0.990000 };
        Camera.farPlane = float{ 1000.000000 };
        Camera.aspectRatio = float{ 1.777778 };
        Camera.fov = float{ 60.000000 };
        auto& DirectionLight = ent.AddComponent<Component::DirectionLight>();
        DirectionLight.lightColor = Math::Vec3{ 1.000000,1.000000,1.000000 };
        DirectionLight.diffuseIntensity = float{ 0.960000 };
        DirectionLight.direction = Math::Vec3{ -1.000000,1.000000,-1.000000 };
        DirectionLight.ambientIntensity = float{ 0.190000 };
        DirectionLight.rimPower = float{ 3.400000 };
        scene->SetCameraEntityHandle(&ent);
    }
}

} // Namespace Scarlet.

#endif // DEV_CONFIGURATION.


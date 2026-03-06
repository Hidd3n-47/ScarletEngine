#include "Game.h"

#include <iostream>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>

#include <ScarletEngine/Components/Transform.h>

void Game::Init()
{
    auto scene = ScarlEnt::Registry::Instance().CreateScene("MainScene");
    ScarlEnt::Registry::Instance().SetActiveScene(scene);
    (void)scene->AddEntity<Scarlet::Component::Transform>();

    std::cout << "Hello from game side.\n";
}

void Game::Terminate()
{
    auto scene = ScarlEnt::Registry::Instance().GetActiveScene();
    ScarlEnt::Registry::Instance().RemoveScene(scene);
    std::cout << "Goodbye from game side.\n";
}

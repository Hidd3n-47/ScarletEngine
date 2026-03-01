#include "Game.h"

#include <iostream>

#include <ScarlEnt/Scene.h>
#include <ScarlEnt/Registry.h>

void Game::Init()
{
    auto scene = ScarlEnt::Registry::Instance().CreateScene("MainScene");
    ScarlEnt::Registry::Instance().SetActiveScene(scene);
    (void)scene->AddEntity<int>(47);

    std::cout << "Hello from game side.\n";
}

void Game::Terminate()
{
    std::cout << "Goodbye from game side.\n";
}

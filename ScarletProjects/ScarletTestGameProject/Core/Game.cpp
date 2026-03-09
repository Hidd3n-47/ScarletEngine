#include "Game.h"

#include <iostream>

#include <ScarlEnt/Scene.h>

void Game::Init()
{
    std::cout << "Hello from game side.\n";
}

void Game::Terminate()
{
    std::cout << "Goodbye from game side.\n";
}

#include "Game.h"

#include <iostream>

#ifndef DEV_CONFIGURATION
#include "Build/SceneBuild.h"
#endif // DEV_CONFIGURATION.

void Game::Init()
{
    std::cout << "Hello from game side.\n";

#ifndef DEV_CONFIGURATION
    Scarlet::InitGameScene();
#endif // DEV_CONFIGURATION.
}

void Game::Terminate()
{
    std::cout << "Goodbye from game side.\n";
}

#include "Game.h"

#include <ScarlEnt/Registry.h>

#include "Scenes/ScarletTestGameProjectScene.h"

#ifndef DEV_CONFIGURATION
#include "Build/SceneBuild.h"
#endif // DEV_CONFIGURATION.

void Game::Init()
{
    ScarlEnt::Registry::Instance().GetOrCreateScene<ScarletTestGameProject>("ScarletTestGameProject");

#ifndef DEV_CONFIGURATION
    Scarlet::InitGameScene();
#endif // DEV_CONFIGURATION.
}

void Game::Terminate()
{
    ScarlEnt::Registry::Instance().RemoveScene("ScarletTestGameProject");
}

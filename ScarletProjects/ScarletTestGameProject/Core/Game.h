#pragma once

#include <Scarlet/Core/IGame.h>

#include "Defines.h"

class GAME_API Game : public Scarlet::IGame
{
public:
    void Init() override;
    void Terminate() override;
private:

};

extern "C" inline GAME_API Scarlet::IGame* CreateGameInstance() 
{
    return new Game();
}
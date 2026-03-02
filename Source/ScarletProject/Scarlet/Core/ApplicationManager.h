#pragma once

#include <Windows.h>

#include <ScarletCore/PrimitiveTypes.h>

namespace Scarlet
{

class IGame;

class ApplicationManager
{
public:
    void Init();
    void Terminate();

    void LoadGameDll();
    void UnloadGameDll();
private:
    typedef IGame* (*CreateGameInstanceFunc)();

    IGame* mGame = nullptr;
    uint32 mLoadedDlls = 0;

    HMODULE mGameDll = nullptr;

    CreateGameInstanceFunc  CreateGameInstanceFunctionPointer;
};

} // Namespace Scarlet.
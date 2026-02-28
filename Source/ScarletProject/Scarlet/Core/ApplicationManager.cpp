#include "ScarletPch.h"
#include "ApplicationManager.h"

#include <Windows.h>

#include "IGame.h"
#include <ScarletCore/Defines.h>

namespace Scarlet
{

void ApplicationManager::Init()
{
    LoadGameDll();
}

void ApplicationManager::Terminate()
{
    if (mGame)
    {
        mGame->Terminate();
        delete mGame;
        mGame = nullptr;
    }
}

void ApplicationManager::LoadGameDll()
{
    if (mGame)
    {
        mGame->Terminate();
        delete mGame;
        mGame = nullptr;
    }

    const HMODULE gameDll = LoadLibrary(L"E:/Programming/ScarletEngine/Scratch/Bin/Dev/ScarletTestGameProject/ScarletTestGameProject.dll");

    if (!gameDll)
    {
        SCARLET_ERROR("Failed to load the game DLL.");
        return;
    }

    // Get function pointers for the create function.
    CreateGameInstanceFunctionPointer = reinterpret_cast<CreateGameInstanceFunc>(GetProcAddress(gameDll, "CreateGameInstance"));

    if (!CreateGameInstanceFunctionPointer)
    {
        SCARLET_ERROR("Failed to get function pointer for creating game instance in DLL.");
        return;
    }

    mGame = CreateGameInstanceFunctionPointer();
    mGame->Init();
}

} // Namespace Scarlet.
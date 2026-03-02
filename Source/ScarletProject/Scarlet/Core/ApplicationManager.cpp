#include "ScarletPch.h"
#include "ApplicationManager.h"

#include <string>

#include <ScarletCore/Defines.h>
#include <ScarletEngine/Core/Engine.h>

#ifdef DEV_CONFIGURATION
#include <ScarletEditor/Core/EditorManager.h>)
#endif // DEV_CONFIGURATION.

#include "IGame.h"

namespace Scarlet
{

void ApplicationManager::Init()
{
    LoadGameDll();

    DEBUG(Editor::EditorManager::Init());

    Engine::Instance().SetReloadDllFunction([&] { LoadGameDll(); });
}

void ApplicationManager::Terminate()
{
    DEBUG(Editor::EditorManager::Destroy());

    UnloadGameDll();
}

void ApplicationManager::LoadGameDll()
{
    UnloadGameDll();

#ifdef DEV_CONFIGURATION
    const std::string fileName = "ScarletTestGameProject.dll";
    const std::string path     = "E:/Programming/ScarletEngine/Scratch/Bin/Dev/ScarletTestGameProject/";
    const std::string tempPath = path + "Game_temp_" + std::to_string(mLoadedDlls - 1) + ".dll";

    if (!CopyFileA((path + fileName).c_str(), tempPath.c_str(), FALSE))
    {
        SCARLET_ERROR("Failed to copy the game DLL to a temporary location.");
        return;
    }

    mGameDll = LoadLibraryA(tempPath.c_str());
#else // DEV_CONFIGURATION.
    const HMODULE gameDll = LoadLibrary(L"E:/Programming/ScarletEngine/Scratch/Bin/Release/ScarletTestGameProject/ScarletTestGameProject.dll");
#endif // !DEV_CONFIGURATION.

    if (!mGameDll)
    {
        SCARLET_ERROR("Failed to load the game DLL.");
        return;
    }

    // Get function pointers for the create function.
    CreateGameInstanceFunctionPointer = reinterpret_cast<CreateGameInstanceFunc>(GetProcAddress(mGameDll, "CreateGameInstance"));

    if (!CreateGameInstanceFunctionPointer)
    {
        SCARLET_ERROR("Failed to get function pointer for creating game instance in DLL.");
        return;
    }

    mGame = CreateGameInstanceFunctionPointer();
    mGame->Init();
}

void ApplicationManager::UnloadGameDll()
{
    if (mGame)
    {
        mGame->Terminate();
        delete mGame;
        mGame = nullptr;

        FreeLibrary(mGameDll);

#ifdef DEV_CONFIGURATION
        const std::string path     = "E:/Programming/ScarletEngine/Scratch/Bin/Dev/ScarletTestGameProject/";
        const std::string tempPath = path + "Game_temp_" + std::to_string(mLoadedDlls - 1) + ".dll";
        DeleteFileA(tempPath.c_str());
#endif // DEV_CONFIGURATION.

    }
}

} // Namespace Scarlet.
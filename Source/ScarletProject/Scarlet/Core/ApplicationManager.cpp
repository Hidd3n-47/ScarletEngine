#include "ScarletPch.h"
#include "ApplicationManager.h"

#include <string>

#include <ScarletCore/Defines.h>
#include <ScarletEngine/Core/Engine.h>

#ifdef DEV_CONFIGURATION
#include <ScarletEditor/Core/EditorManager.h>
#endif // DEV_CONFIGURATION.

#include "IGame.h"
#include "ScarletEngine/Core/Filepath.h"

namespace Scarlet
{

void ApplicationManager::Init()
{
#ifndef DEV_CONFIGURATION
    LoadGameDll();
#endif // ^^^ !DEV_CONFIGURATION. ^^^

    DEBUG(Engine::Instance().SetReloadDllFunction ([&] { LoadGameDll(); }));
    DEBUG(Engine::Instance().SetReloadGameFunction([&] { ReloadGame();  }));

    DEBUG(Editor::EditorManager::Init());
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
    const std::string projectName = Editor::EditorManager::Instance().GetProjectName();
    const std::string path        = Filepath{ FilepathDirectory::PROJECT, "Scratch/Bin/Dev/" + projectName + "/" }.GetAbsolutePath();
    mTempDllPath = path + "Game_temp_" + std::to_string(mLoadedDlls++) + ".dll";

    if (!CopyFileA((path + projectName + ".dll").c_str(), mTempDllPath.c_str(), FALSE))
    {
        SCARLET_ERROR("Failed to copy the game DLL to a temporary location.");
        return;
    }

    mGameDll = LoadLibraryA(mTempDllPath.c_str());
#else // DEV_CONFIGURATION.
    mGameDll = LoadLibrary(L"Game.dll");
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
        if (!mTempDllPath.empty())
        {
            DeleteFileA(mTempDllPath.c_str());
        }

        mTempDllPath = "";
#endif // DEV_CONFIGURATION.

    }
}

void ApplicationManager::ReloadGame() const
{
    if (mGame)
    {
        mGame->Terminate();
        mGame->Init();
    }
}

} // Namespace Scarlet.

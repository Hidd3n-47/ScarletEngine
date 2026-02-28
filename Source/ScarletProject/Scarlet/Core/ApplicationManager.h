#pragma once

namespace Scarlet
{

class IGame;

class ApplicationManager
{
public:
    void Init();
    void Terminate();

    void LoadGameDll();
private:
    typedef IGame* (*CreateGameInstanceFunc)();

    IGame* mGame;

    CreateGameInstanceFunc  CreateGameInstanceFunctionPointer;
};

} // Namespace Scarlet.
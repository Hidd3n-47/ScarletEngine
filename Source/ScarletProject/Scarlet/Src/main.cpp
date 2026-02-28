#include "ScarletPch.h"

#include <ScarletEngine/Core/Engine.h>

#include "Core/ApplicationManager.h"

int main(int, char**)
{
    Scarlet::Engine::CreateEngine();

    Scarlet::Engine::Instance().Init();

    Scarlet::ApplicationManager* app = new Scarlet::ApplicationManager();
    app->Init();

    Scarlet::Engine::Instance().Run();

    app->Terminate();
    delete app;

    Scarlet::Engine::Instance().Destroy();
}
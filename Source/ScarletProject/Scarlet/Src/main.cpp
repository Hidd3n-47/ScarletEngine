#include "ScarletPch.h"

#include <ScarletEngine/Core/Engine.h>

int main(int, char**)
{
    Scarlet::Engine::CreateEngine();

    Scarlet::Engine::Instance().Init();
    Scarlet::Engine::Instance().Run();
    Scarlet::Engine::Instance().Destroy();
}
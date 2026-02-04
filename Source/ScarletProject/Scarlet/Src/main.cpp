#include "ScarletPch.h"

#include <ScarletEngine/Core/Engine.h>

int main(int, char**)
{
    Scarlet::Engine::Init();
    Scarlet::Engine::Instance().Run();
    Scarlet::Engine::Destroy();
}
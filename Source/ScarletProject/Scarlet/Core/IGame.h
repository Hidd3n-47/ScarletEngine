#pragma once

namespace Scarlet
{

class IGame
{
public:
    virtual ~IGame() = default;

    virtual void Init() = 0;
    virtual void Terminate() = 0;
};

} // Namespace Scarlet.
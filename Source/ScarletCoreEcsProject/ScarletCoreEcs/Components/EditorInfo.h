#pragma once

#include <ScarlEnt/RTTI/Rtti.h>

namespace Scarlet::Component
{

struct EditorInfo
{
    std::string name = "Entity";
    bool isMutable   = true;

    SCARLET_COMPONENT(EditorInfo)
};

} // Namespace Scarlet::Component.
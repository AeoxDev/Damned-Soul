#pragma once
#include "EntityID.h"

namespace GALE_AMULET
{
    extern EntityID _OWNER;

    const char* Description();

    // Call to set active for a specific entity (usually player)
    void Initialize(void* input);

    void IncreaseSpeed(void* data);
}
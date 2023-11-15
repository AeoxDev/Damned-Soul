#pragma once
#include "EntityID.h"

namespace SPEEDY_LITTLE_DEVIL
{
    extern EntityID _OWNER;

    const char* Description();

    // Call to set active for a specific entity (usually player)
    void Initialize(void* input);

    void IncreaseSpeed(void* data);
}
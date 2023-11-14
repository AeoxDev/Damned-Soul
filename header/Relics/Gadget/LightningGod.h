#pragma once
#include "EntityID.h"

namespace LIGHTNING_GOD
{
    extern EntityID _OWNER;

    const char* Description();

    // Call to set active for a specific entity (usually player)
    void Initialize(void* input);

    // Deal damage to marked entity
    void ProcMark(void* data);

    // Update, cooldown, and mark
    void OnUpdate(void* data);
}
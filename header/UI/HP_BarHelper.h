#pragma once
#include "EntityID.h"//"Components/UIComponents/UIComponent.h"
#include <cinttypes>

void SetUpAdvancedHealthBar(const EntityID player);

void ScaleAdvancedHealthBar(const EntityID player);

void UpdateSoulUI(const EntityID player);

uint32_t AddNewRelicToUI(const EntityID player, const void* relic);

// Recompute anchors after editor changed positions
void RecomputeHPBarAnchors(const EntityID player);
void RecomputeAllHPBarAnchors();
#pragma once
#include "EntityID.h"//"Components/UIComponents/UIComponent.h"

void SetUpAdvancedHealthBar(const EntityID player);

void ScaleAdvancedHealthBar(const EntityID player);

void UpdateSoulUI(const EntityID player);

void AddNewRelicToUI(const EntityID player, const void* relic);
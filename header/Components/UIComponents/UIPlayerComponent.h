#pragma once
#include "Relics/RelicFunctions.h"

#define MAX_DISPLAY_RELICS (15)

struct UIPlayerRelicsComponent
{
	const int maxRelics = MAX_DISPLAY_RELICS;
	int currentRelics = 0;

	const RelicData* relics[MAX_DISPLAY_RELICS] = {};

	UIPlayerRelicsComponent() = default;
};

struct UIPlayerSoulsComponent
{
	int value = -1;
	int spentThisShop = 0;
	int spentThisShopOnRelics = 0;
};
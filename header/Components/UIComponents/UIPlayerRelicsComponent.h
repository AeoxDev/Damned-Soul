#pragma once
#include "Relics/RelicFunctions.h"

#define MAX_DISPLAY_RELICS (15)

struct UIPlayerRelicsComponent
{
	const int maxRelics = MAX_DISPLAY_RELICS;
	int currentRelics = 0;
	
	const RelicData* relics[MAX_DISPLAY_RELICS] = {};

	DSINT2 gridPos = { 0, 0 };

	UIPlayerRelicsComponent() = default;
};
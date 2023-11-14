#pragma once

struct UIPlayerRelicsComponent
{
	DSINT2 gridPos = { 0, 0 };
	const int maxRelics = 21;
	int currentRelics = 0;
};
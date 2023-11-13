#pragma once
#include "Relics/RelicFunctions.h"

#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

enum shopState
{
	IGNORED = -1,
	AVALIABLE,
	SELECTED,
	BOUGHT,
	LOCKED,

};

struct UIRelicWindowComponent
{
	shopState shopSelections[2] = {};
	const RelicData* shopRelics[2] = {};

	UIRelicWindowComponent()
	{
		shopSelections[0] = AVALIABLE;
		shopSelections[1] = AVALIABLE;
	};

};

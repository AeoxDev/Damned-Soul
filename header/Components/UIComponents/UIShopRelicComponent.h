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

struct UIShopRelicComponent
{
	ML_Array<shopState, 2> shopSelections;
	ML_Array<const RelicData*, 2> shopRelics;

	UIShopRelicComponent()
	{
		shopSelections[0] = AVALIABLE;
		shopSelections[1] = AVALIABLE;
	};

	void Release()
	{
		shopSelections.~ML_Array();
		shopRelics.~ML_Array();
	};
};

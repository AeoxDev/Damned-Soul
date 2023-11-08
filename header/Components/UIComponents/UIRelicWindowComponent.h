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
	ML_Array<shopState, 2> shopSelections;
	ML_Array<const RelicData*, 2> shopRelics;

	UIRelicWindowComponent()
	{
		shopSelections[0] = AVALIABLE;
		shopSelections[1] = AVALIABLE;
	};

	void Release()
	{
		shopRelics.~ML_Array();
		shopSelections.~ML_Array();
	};

};

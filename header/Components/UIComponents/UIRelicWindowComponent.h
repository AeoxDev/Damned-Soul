#pragma once
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

	UIRelicWindowComponent()
	{
		shopSelections[0] = AVALIABLE;
		shopSelections[1] = AVALIABLE;
	};

};

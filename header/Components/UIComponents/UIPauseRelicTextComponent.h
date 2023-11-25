#pragma once
#include "MemLib/ML_String.hpp"

struct UIPauseRelicTextComponent
{
	ML_String name;
	ML_String description;

	void Release()
	{
		name.~ML_String();
		description.~ML_String();
	};
};
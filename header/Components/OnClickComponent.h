#pragma once
#include "UI.h"

#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_Array.hpp"

struct OnClickComponent
{
	ML_Vector<DSFLOAT2> positions;
	ML_Vector<DSBOUNDS> bounds;

	int index = 0;

	//2d container of click functions, 0 for mouse pressed and 1 for mouse released
	ML_Vector<void(*)(void*, int)> onClickFunctionsPressed;
	ML_Vector<void(*)(void*, int)> onClickFunctionsReleased;

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, void(*press)(void*, int), void(*release)(void*, int))
	{
		positions.Initialize();
		bounds.Initialize();

		onClickFunctionsPressed.Initialize();
		onClickFunctionsReleased.Initialize();

		positions.push_back(pos);
		bounds.push_back(bnds);

		onClickFunctionsPressed.push_back(press);
		onClickFunctionsReleased.push_back(release);
	};

	void Add(DSFLOAT2 pos, DSBOUNDS bnds, void(*press)(void*, int), void(*release)(void*, int))
	{
		positions.push_back(pos);
		bounds.push_back(bnds);

		onClickFunctionsPressed.push_back(press);
		onClickFunctionsReleased.push_back(release);
	};

	//Returns -1 for no intersect, positive number for first index that interacted, 
	int Intersect(DSINT2 mousePosition)
	{
		int retval = -1;
		for (uint32_t i = positions.size(); i-- > 0;)
		{
			if ((mousePosition.x > positions[i].x) && (mousePosition.x < positions[i].x + bounds[i].right) &&
				(mousePosition.y > positions[i].y) && (mousePosition.y < positions[i].y + bounds[i].bottom))
			{
				retval = (int)i;
				index = (int)i;
				break;
			}
		}

		return retval;
	};

	void Release()
	{
		positions.~ML_Vector();
		bounds.~ML_Vector();

		onClickFunctionsPressed.~ML_Vector();
		onClickFunctionsReleased.~ML_Vector();
	};
};
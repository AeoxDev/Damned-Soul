#pragma once
#include "UI.h"

#include "MemLib/ML_Vector.hpp"

#include <vector>

struct OnClickComponent
{
	ML_Vector<DSFLOAT2> positions;
	ML_Vector<DSBOUNDS> bounds;

	int index = 0;

	//container of bool doesnt exist, 0 = pressed, 1 = released
	ML_Vector<int> mouseStates;

	ML_Vector<void(*)(void*, int)> onClickFunctions;

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, int state, void(*func)(void*, int))
	{
		positions.Initialize();
		bounds.Initialize();
		mouseStates.Initialize();
		onClickFunctions.Initialize();

		positions.push_back(pos);
		bounds.push_back(bnds);

		mouseStates.push_back(state);

		onClickFunctions.push_back(func);
	};

	void Add(DSFLOAT2 pos, DSBOUNDS bnds, int state, void(*func)(void*, int))
	{
		positions.push_back(pos);
		bounds.push_back(bnds);

		mouseStates.push_back(state);

		onClickFunctions.push_back(func);
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
		mouseStates.~ML_Vector();
		onClickFunctions.~ML_Vector();
	};
};
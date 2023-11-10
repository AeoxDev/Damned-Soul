#pragma once
#include "UI.h"

#include "MemLib/ML_Vector.hpp"

#include <vector>

struct OnClickComponent
{
	std::vector<DSFLOAT2> positions;
	std::vector<DSBOUNDS> bounds;

	int index = 0;

	std::vector<void(*)(void*, int)> onClickFunctions;

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, void(*func)(void*, int))
	{
		positions.push_back(pos);
		bounds.push_back(bnds);

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
				retval = i;
				index = i;
				break;
			}
		}

		return retval;
	};

	void Release()
	{
		positions.~vector();
		bounds.~vector();

		onClickFunctions.~vector();
	};
};
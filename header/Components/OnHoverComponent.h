#pragma once
#include "UI.h"

#include "MemLib/ML_Vector.hpp"

struct OnHoverComponent
{
	ML_Vector<DSFLOAT2> positions;
	ML_Vector<DSBOUNDS> bounds;

	int index = -1;

	bool redrawUI = true;
	bool hasBeenDrawn = false;

	void(*onHover)(void*, int, bool) = nullptr;

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, void(*func)(void*, int, bool))
	{
		AddElement(pos, bnds);
		onHover = func;
	};

	void AddElement(DSFLOAT2 pos, DSBOUNDS bnds)
	{
		positions.push_back(pos);
		bounds.push_back(bnds);
	};

	//Returns -1 for no intersect, positive number for first index that interacted, 
	int Intersect(DSINT2 mousePosition)
	{
		int retval = -1;
		for (uint32_t i = 0; i < positions.size(); i++)
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
		positions.~ML_Vector();
		bounds.~ML_Vector();
	};
};
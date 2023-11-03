#pragma once
#include "UI.h"

struct OnClickComponent
{
	ML_Vector<DSFLOAT2> positions;
	ML_Vector<DSBOUNDS> bounds;

	int index = -1;

	void(*onClick)(void*, int);

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, void(*func)(void*, int))
	{
		AddElement(pos, bnds);
		onClick = func;
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
#pragma once
#include "UI.h"

#include "MemLib/ML_Vector.hpp"

struct OnHoverComponent
{
	ML_Vector<DSFLOAT2> positions;
	ML_Vector<DSBOUNDS> bounds;

	int index = 0;
	int oldIndex = 0;

	//container of bool doesnt exit, 0 = false, 1 = true
	ML_Vector<int> redrawUIChecks;

	//container of bool doesnt exit, 0 = false, 1 = true
	ML_Vector<int> hasBeenDrawnChecks;

	ML_Vector<void(*)(void*, int, bool)> onHoverFunctions;

	void Setup(DSFLOAT2 pos, DSBOUNDS bnds, void(*func)(void*, int, bool))
	{
		positions.Initialize();
		bounds.Initialize();

		redrawUIChecks.Initialize();
		hasBeenDrawnChecks.Initialize();

		onHoverFunctions.Initialize();

		positions.push_back(pos);
		bounds.push_back(bnds);

		onHoverFunctions.push_back(func);

		redrawUIChecks.push_back(1);
		hasBeenDrawnChecks.push_back(0);
	};

	void Add(DSFLOAT2 pos, DSBOUNDS bnds, void(*func)(void*, int, bool))
	{
		positions.push_back(pos);
		bounds.push_back(bnds);

		onHoverFunctions.push_back(func);

		redrawUIChecks.push_back(1);
		hasBeenDrawnChecks.push_back(0);
	};

	//Returns -1 for no intersect, positive number for newest interactable.
	int Intersect(DSINT2 mousePosition)
	{
		int retval = -1;
		for (uint32_t i = positions.size(); i --> 0;)
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

		redrawUIChecks.~ML_Vector();
		hasBeenDrawnChecks.~ML_Vector();

		onHoverFunctions.~ML_Vector();
	};
};
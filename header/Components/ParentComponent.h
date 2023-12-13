#pragma once
#include "EntityID.h"


struct ParentComponent
{
	EntityID parentID;
	int parentCheck;

	ParentComponent(const EntityID& ID, const int& check)
	{
		parentID = ID;
		parentCheck = check;
	}
};
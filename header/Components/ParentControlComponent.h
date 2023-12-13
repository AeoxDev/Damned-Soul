#pragma once

struct ParentControlComponent
{
	int childCheck;

	ParentControlComponent(const int& ID)
	{
		childCheck = ID;
	}
};
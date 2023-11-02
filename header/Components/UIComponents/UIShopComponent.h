#pragma once
#include "UIElement.h"
#include "UIButtonFunctions.h"
#include "RelicFunctions.h"

#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

#include "Registry.h"

struct EntityID;

struct UIShopRelicWindowComponent
{
	bool doRedraw = true;
	int shopPosition = -1;
	bool bought = false;

};

struct UIShopComponent
{
	bool ignore = true;

	void Setup();
};


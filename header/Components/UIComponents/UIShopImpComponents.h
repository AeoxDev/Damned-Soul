#pragma once
#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_String.hpp"

struct UIShopTitleImpComponent
{
	ML_String name;
};

struct UIShopImpComponent
{
	ML_String name;
	ML_String description;
	uint8_t price;
};
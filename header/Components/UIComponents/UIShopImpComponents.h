#pragma once
#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_String.hpp"

struct UIShopTitleImpComponent
{
	ML_String name;

	void Release() { name.~ML_String(); };
};

struct UIShopImpComponent
{
	ML_String name;
	ML_String description;
	uint8_t price;

	void Release() 
	{ 
		name.~ML_String(); 
		description.~ML_String();
	};
};
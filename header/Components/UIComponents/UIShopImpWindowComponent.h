#pragma once
#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_String.hpp"

struct UIShopImpWindowComponent
{
	ML_String title;
	ML_String description;
	ML_Vector<ML_String> subdescriptions;
};
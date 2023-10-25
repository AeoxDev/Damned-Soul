#pragma once
#include "MemLib/ML_Vector.hpp"
#include <fstream>

struct ProximityPointEdit
{
	float x;
	float z;
};

struct CurrentHitboxVariables
{
	int clockwise = 1; //Clockwise or counter-clockwise
	ML_Vector<ProximityPointEdit> list;
};

void HitboxEditorLoop(const char* fileName, bool clockwise = true);

void EdgeCaseFixer(CurrentHitboxVariables& CHV, bool lastIncluded);
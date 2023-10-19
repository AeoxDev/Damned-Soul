#pragma once
#include "MemLib/ML_Vector.hpp"
#include <string>
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

void HitboxEditorLoop(std::string fileName, bool clockwise = true);

void EdgeCaseFixer(CurrentHitboxVariables& CHV, bool lastIncluded);
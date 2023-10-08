#pragma once
#include "MemLib/ML_Vector.hpp"
#include <string>
#include <fstream>

struct ProximityPoint
{
	float x;
	float z;
};

struct CurrentHitboxVariables
{
	int clockwise = 1; //Clockwise or counter-clockwise
	ML_Vector<ProximityPoint> list;
};

void HitboxEditorLoop(std::string fileName, bool clockwise = true);

void EdgeCaseFixer(CurrentHitboxVariables& CHV, bool lastIncluded);
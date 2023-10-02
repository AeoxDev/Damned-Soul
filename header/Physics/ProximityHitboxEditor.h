#pragma once
#include "MemLib/ML_Vector.hpp"
#include <string>
#include <fstream>

struct ProximityPoint
{
	int index;
	float x;
	float z;
};

struct CurrentHitboxVariables
{
	int clockwise = 1; //Clockwise or counter-clockwise
	ML_Vector<ProximityPoint> pointList;
	std::string fileName = "";
};

void HitboxEditorLoop();


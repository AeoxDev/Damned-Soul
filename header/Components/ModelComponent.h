#pragma once
#include "Model.h"
#include "MemLib\ML_Vector.hpp"

struct ModelBonelessComponent
{
	uint64_t model;
	//Multiplier on color result before gamma correction
	float colorMultiplicativeRed = 1.0f, colorMultiplicativeGreen = 1.0f, colorMultiplicativeBlue = 1.0f;
	//Additive on color result before color correction
	float colorAdditiveRed = 0.0f, colorAdditiveGreen = 0.0f, colorAdditiveBlue = 0.0f;
	//int model;
	//Add more stuff for models
	//int texture;
};

struct ModelSkeletonComponent
{
	uint64_t model;
};
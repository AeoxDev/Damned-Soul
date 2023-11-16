#pragma once
//#include "Model.h"
//#include "MemLib\ML_Vector.hpp"
#include <stdint.h>
struct ModelBonelessComponent
{
	uint64_t model;
	//Multiplier on color result before gamma correction
	float colorMultiplicativeRed = 1.0f, colorMultiplicativeGreen = 1.0f, colorMultiplicativeBlue = 1.0f;
	//Additive on color result before color correction
	float colorAdditiveRed = 0.0f, colorAdditiveGreen = 0.0f, colorAdditiveBlue = 0.0f;

	//Gamma correction per model.
	float gammaCorrection = 1.0f;
	//int model;
	//Add more stuff for models
	//int texture;

	float baseColorMultiplicativeRed = 1.0f, baseColorMultiplicativeGreen = 1.0f, baseColorMultiplicativeBlue = 1.0f;
	float baseColorAdditiveRed = 0.0f, baseColorAdditiveGreen = 0.0f, baseColorAdditiveBlue = 0.0f;
	bool castShadow = true;
};

struct ModelSkeletonComponent
{
	uint64_t model;
	float colorMultiplicativeRed = 1.0f, colorMultiplicativeGreen = 1.0f, colorMultiplicativeBlue = 1.0f;
	//Additive on color result before color correction
	float colorAdditiveRed = 0.0f, colorAdditiveGreen = 0.0f, colorAdditiveBlue = 0.0f;
	//Gamma correction per model.
	float gammaCorrection = 1.0f;

	//base values for hue (hue hue hue hue)
	float baseColorMultiplicativeRed = 1.0f, baseColorMultiplicativeGreen = 1.0f, baseColorMultiplicativeBlue = 1.0f;
	float baseColorAdditiveRed = 0.0f, baseColorAdditiveGreen = 0.0f, baseColorAdditiveBlue = 0.0f;
};
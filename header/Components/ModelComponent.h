#pragma once
#include "Model.h"
#include "MemLib\ML_Vector.hpp"

int LoadModel(const char* filename);


struct ModelBonelessComponent
{
	Model model;
	//int model;
	//Add more stuff for models
	//int texture;
};

struct ModelSkeletonComponent
{
	Model model;
};
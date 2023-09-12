#pragma once
#include "SceneEnums.h"

struct Shop
{
	std::string id = "Shop";
	int i = 0;

	void Update();

	void WriteEntities(Registry& registry);
};
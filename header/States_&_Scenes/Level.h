#pragma once
#include "SceneEnums.h"

struct Level
{
	std::string id;
	std::vector<std::string> entityList;

	int i = 0;

	Level(std::string id, Registry& registry, std::vector<std::string> entityList);

	void WriteEntities(Registry& registry);

	void Update();

	void ResetEntities(Registry& registry);
};
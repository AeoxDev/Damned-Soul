#pragma once
#include <string>
#include <iostream>

#include "EntityFramework.h"

enum class Scene
{
	Level,
	Shop
};

struct PlayerComponent
{
	int hp = 0;

	PlayerComponent(int hp) : hp(hp) {}
};

struct ImpComponent
{
	int hp = 0;

	ImpComponent(int hp) : hp(hp) {}
};


struct SkeletonComponent
{
	int hp = 0;

	SkeletonComponent(int hp) : hp(hp) {}
};


struct DemonComponent
{
	int hp = 0;

	DemonComponent(int hp) : hp(hp) {}
};

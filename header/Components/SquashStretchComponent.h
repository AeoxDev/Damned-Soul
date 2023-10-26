#pragma once
#include "EntityID.h"
enum SquashStretchType
{
	Constant,
	Linear,
	Accelerating,
	//Decelerating,
	//SinusCurve//accelerating and deccelerating
};

struct SquashStretchComponent
{
	bool useStartScale = false;
	float startScaleX = 1.0f, startScaleY = 1.0f, startScaleZ = 1.0f;
	float goalScaleX = 1.0f, goalScaleY = 1.0f, goalScaleZ = 1.0f;
	SquashStretchType type;
};
//NOTE: You still need to add a timedEvent component to make use of these values.
SquashStretchComponent* AddSquashStretch(EntityID& entity, const SquashStretchType type, const float& goalX, const float& goalY, const float& goalZ, const bool& useStartScale = false, const float& startX = 1.0f, const float& startY = 1.0f, const float& startZ = 1.0f);
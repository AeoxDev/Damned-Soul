#pragma once

#include "Components.h"
#include "GeometryIndependentComponent.h"

struct PathfindingMap
{
	float grid[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS] = { 0 };
};

float Calculate2dDistance(float pos1X, float pos1Z, float pos2X, float pos2Z);
//void SmoothRotation(TransformComponent* ptc, float goalX, float goalZ, float rotationFactor = 5.1f);
PathfindingMap CalculateGlobalMapValuesSkeleton(EntityID& mapID, TransformComponent* playerTransform);
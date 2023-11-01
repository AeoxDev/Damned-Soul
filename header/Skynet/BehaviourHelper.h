#pragma once

#include "Components.h"
#include "GeometryIndependentComponent.h"
#include "MemLib\ML_Vector.hpp"

struct PathfindingMap //costs of tiles on map
{
	float cost[GI_TEXTURE_DIMENSIONS][GI_TEXTURE_DIMENSIONS] = { 0 };

	~PathfindingMap() = default;
};


struct Node // to save path
{
	int x = 0; 
	int z = 0;
	float g = 0; // grid cost
	float h = 0; // manhattan distance
	float f = 0; // total cost

	int parentX, parentZ;

	~Node() = default;
};

float Calculate2dDistance(float pos1X, float pos1Z, float pos2X, float pos2Z);
//void SmoothRotation(TransformComponent* ptc, float goalX, float goalZ, float rotationFactor = 5.1f);
PathfindingMap CalculateGlobalMapValuesSkeleton(EntityID& mapID);

ML_Vector<Node> CalculateAStarPath(EntityID& mapID, PathfindingMap gridValues, TransformComponent* enemyTransform, TransformComponent* playerTransform);
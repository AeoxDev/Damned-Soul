#pragma once

#include "Model.h"

struct PlayerComponent
{
	//Should be moved to a TransformComponent
	//Position
	float posX = 0.f, posY = 0.f, posZ = 0.f;
	float movementSpeed = 20.0f;
	//Rotation
	float dirX = 0.0f, dirZ = 1.0f;//Facing direction
	float goalX = 0.0f, goalZ = -1.0f;//Goal direction

	//Scale

	//Controller component
};
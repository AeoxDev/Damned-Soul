#pragma once

#include "Model.h"

struct PlayerComponent
{
	//Should be moved to a TransformComponent
	//Position
	float movementSpeed = 20.0f;
	//Rotation
	float goalX = 0.0f, goalZ = -1.0f;//Goal direction

	//Scale

	//Controller component
};
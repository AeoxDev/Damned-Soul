#pragma once

#include "Model.h"

struct PlayerComponent
{
	//Should be moved to a TransformComponent
	//Position
	float movementSpeed = 20.0f;
	//Rotation
	float goalX = 0.0f, goalZ = -1.0f;//Goal direction
	float health = 10.0f;
	int souls = 4;
	//Scale

	//Controller component
	float moveTime = .0f;//This is the time the player has been moving, used for camera feel.
	float moveFactor = 2.5f;
	float moveResetFactor = 1.25f;
	float moveMaxLimit = 3.0f;
};
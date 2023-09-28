#pragma once

#include "Model.h"

struct PlayerComponent
{
	//Should be moved to a TransformComponent
	//Position
	float posX = 0.f, posY = 0.f, posZ = 0.f;

	//Rotation

	//Scale

	//Model (Should be moved)
	Model* model;

	//Controller component
};
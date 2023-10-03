#pragma once

struct TransformComponent
{
	//Pos
	float positionX = 0.f, positionY = 0.f, positionZ = .0f;
	float lastPositionX = 0.f, lastPositionY = 0.f, lastPositionZ = .0f;
	//Rotation
	//What direction in coordinates
	float facingX = 0.f, facingY = 0.f, facingZ = 1.0f;
	//Scale
	float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
	
};
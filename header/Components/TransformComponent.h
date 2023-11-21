#pragma once
#include "EntityID.h"

struct TransformComponent
{
	//Pos
	float positionX = 0.f, positionY = 0.f, positionZ = .0f;
	//Offset for special effects, does only affect rendering.
	float offsetX = 0.0f, offsetY = 0.0f, offsetZ = 0.0f;
	float lastPositionX = 0.f, lastPositionY = 0.f, lastPositionZ = .0f;
	//Rotation
	//What direction in coordinates
	float facingX = 0.f, facingY = 0.f, facingZ = -1.0f;
	//Scale
	float scaleX = 1.0f, scaleY = 1.0f, scaleZ = 1.0f;
	float offsetScaleX = 1.0f, offsetScaleY = 1.0f, offsetScaleZ = 1.0f;
	//Mass
	float mass = 1.0f;//Used for moveable collision (clipping collision)

	//Force for ice hazard
	float currentSpeedX = 0.0f, currentSpeedZ = 0.0f;
};

float DistanceBetweenTransforms(TransformComponent* f, TransformComponent* s);

void SmoothRotation(TransformComponent* ptc, float goalX, float goalZ, float rotationFactor = 10.1f);

//This already takes statcomponent into account
void TransformDecelerate(EntityID& entity);
//This already takes statcomponent into account, x, z is direction.
void TransformAccelerate(EntityID& entity, float x, float z);
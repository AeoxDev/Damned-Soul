#pragma once
//#include "EntityFramework.h"

enum LightType
{
	inactive,
	pointLight,
	spotlight
};

struct EntityID;

struct LightComponent;

void SetDirectionLight(float colorRed, float colorGreen, float colorBlue, float directionX, float directionY, float directionZ);
//Point light
int CreatePointLight(EntityID& entity, float colorRed, float colorGreen, float colorBlue, float positionX, float positionY, float positionZ, float range, float fallofFactor);
//Spot light, angle is in degrees, not radians
int CreateSpotLight(EntityID& entity, float colorRed, float colorGreen, float colorBlue, float positionX, float positionY, float positionZ, float range, float fallofFactor,
	float directionX, float directionY, float directionZ, float angle);

void RemoveLight(EntityID& entity);
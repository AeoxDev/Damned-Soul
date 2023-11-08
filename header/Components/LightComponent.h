#pragma once
//#include "EntityFramework.h"

enum LightType
{
	inactive,
	pointLight,
	spotlight
};

struct float3
{
	float x, y, z;
};

struct EntityID;

struct LightComponent;

void SetDirectionLight(float colorRed, float colorGreen, float colorBlue, float directionX, float directionY, float directionZ);
//Point light
int CreatePointLight(EntityID& entity, float colorRed, float colorGreen, float colorBlue, float positionX, float positionY, float positionZ, float range, float fallofFactor);
//Spot light, angle is in degrees, not radians
int CreateSpotLight(EntityID& entity, float colorRed, float colorGreen, float colorBlue, float positionX, float positionY, float positionZ, float range, float fallofFactor,
	float directionX, float directionY, float directionZ, float angle);

//Getters
float3 GetLightPosition(const EntityID& entity);
float3 GetLightColor(const EntityID& entity);
float3 GetLightDirection(const EntityID& entity);
float GetLightRange(const EntityID& entity);
float GetLightFallofFactor(const EntityID& entity);
float GetLightAngle(const EntityID& entity);

//Setters
void SetLightPosition(const EntityID& entity, const float& x, const float& y, const float& z);
void SetLightColor(const EntityID& entity, const float& x, const float& y, const float& z);
void SetLightDirection(const EntityID& entity, const float& x, const float& y, const float& z);
void SetLightRange(const EntityID& entity, const float& range);
void SetLightFallofFactor(const EntityID& entity, const float& fallof);
void SetLightAngle(const EntityID& entity, const float& cone);//Angle in degrees

void RemoveLight(EntityID& entity);

void OffsetPosition(EntityID& entity, const float& x, const float& y, const float& z, const float& fx, const float& fy, const float& fz);
void OffsetFacing(EntityID& entity, const float& x, const float& y, const float& z);
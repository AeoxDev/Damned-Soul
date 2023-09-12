#pragma once
#include "EntityFramework.h"
#include "Backend/Collision.h"

//Format
//void(*CollisionFunction)(Registry& registry, EntityID& entity1,
//  EntityID& entity2, const float& normal1X, const  float& normal1Z,
//  const  float& normal2X, const  float& normal2Z);

void NoCollision(OnCollisionParameters& params);

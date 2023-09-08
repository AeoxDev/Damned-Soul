#pragma once
#include "EntityFramework.h"
#include <cmath>
#define CIRCLE_COMPONENT_INDEX = 2 //change to correct number later

struct onCollision
{
	void(*CollisionFunction)(Entity& entity, Entity& entity2, float angleOfAttack1X, float angleOfAttack1Y, float angleOfAttack2X, float angleOfAttack2Y);
};



bool IsCircularCollision(const Entity &entity1, const Entity &entity2); //checks collision between 2 circular hitboxes

void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision


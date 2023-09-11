#pragma once
#include "EntityFramework.h"
#include <cmath>
#define CIRCLE_COMPONENT_INDEX = 2 //change to correct number later

struct onCollision
{
	void(*CollisionFunction)(Registry& registry1, Registry& registry2, EntityID& entity1, EntityID& entity2, float angleOfAttack1X, float angleOfAttack1Y, float angleOfAttack2X, float angleOfAttack2Y);
};



bool IsCircularCollision(const Registry& registry1, const Registry& registry2, const EntityID& entity1, const EntityID& entity2, int circle1, int circle2); //checks collision between 2 circular hitboxes

//void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision


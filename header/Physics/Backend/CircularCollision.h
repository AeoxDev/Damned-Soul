#pragma once
#include "EntityFramework.h"
#include <cmath>
#define CIRCLE_COMPONENT_INDEX = 2 //change to correct number later




bool IsCircularCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circle1, int circle2); //checks collision between 2 circular hitboxes

//void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision


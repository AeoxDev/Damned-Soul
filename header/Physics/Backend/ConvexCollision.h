#pragma once
#include "EntityFramework.h"

bool IsCircularConvexCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circle, int convex); //checks collision between 2 circular hitboxes
bool IsConvexCircularCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int convex, int circle); //checks collision between 2 circular hitboxes

bool IsConvexCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circle1, int circle2); //checks collision between 2 circular hitboxes

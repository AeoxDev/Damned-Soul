#pragma once
#include "EntityFramework.h"

bool IsCircularConvexCollision(EntityID& entity1, EntityID& entity2, int circle, int convex); //checks collision between 2 convex hitboxes
bool IsConvexCircularCollision(EntityID& entity1, EntityID& entity2, int convex, int circle); //checks collision convex and circular hitboxes

bool IsConvexCollision(EntityID& entity1, EntityID& entity2, int circle1, int circle2); //checks collision between 2 circular hitboxes

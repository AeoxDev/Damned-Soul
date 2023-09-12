#pragma once
#include "EntityFramework.h"

bool IsCircularCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circle1, int circle2); //checks collision between 2 circular hitboxes
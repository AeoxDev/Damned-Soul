#pragma once
#include "EntityFramework.h"

int GetOrientation(float Ax, float Az, float Bx, float Bz, float Cx, float Cz); //Return the orientation of the three points
bool IsProximityCorrectSide(Registry& registry, EntityID& wall, int index, float x, float z); //Checks the entity is on the correct side of the wall
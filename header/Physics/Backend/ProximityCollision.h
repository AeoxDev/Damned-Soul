#pragma once
#include "Physics\Backend\Collision.h"

int GetOrientation(float& Ax, float& Az, float& Bx, float& Bz, float& Cx, float& Cz); //Return the orientation of the three points

void FindIntersection(float& x, float& z, ProximityPoint& p1, ProximityPoint& p2, float& otherX, float& otherZ);

void ProximityCorrection(EntityID& wall, int& index, float& x, float& z, float& previousX, float& previousZ); //Checks the entity is on the correct side of the wall

void ProximityMove(ProximityPoint& p1, ProximityPoint& p2, 
		float& x, float& z); //Find normal of line 1 -> 2 and Move entity along the normal based on magnitude.

void ProximityStepChart(ProximityPoint& A, ProximityPoint& B, ProximityPoint& C, 
		float& x, float& z, float& px, float& pz); //Go through the steps of how to handle the collision if entity is on the wrong side of both lines.
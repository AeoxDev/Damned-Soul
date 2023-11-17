#pragma once
#include "Physics\Backend\Collision.h"

void FindIntersection(float& x, float& z, ProximityPoint& p1, ProximityPoint& p2, float& otherX, float& otherZ);

bool IntersectionOnLine(float& line1x1, float line1x2, float& line1z1, float line1z2, float& line2x1, float& line2x2, float& line2z1, float& line2z2);

void ProximityCorrection(EntityID& wall, int& index, float& x, float& z, float& previousX, float& previousZ); //Checks the entity is on the correct side of the wall

void ProximityMove(ProximityPoint& p1, ProximityPoint& p2, 
		float& x, float& z); //Find normal of line 1 -> 2 and Move entity along the normal based on magnitude.

void ProximityStepChart(ProximityPoint& A, ProximityPoint& B, ProximityPoint& C, 
		float& x, float& z, float& px, float& pz); //Go through the steps of how to handle the collision if entity is on the wrong side of both lines.

void ProximityCornerMove(ProximityPoint& A, ProximityPoint& B, ProximityPoint& C, float& x, float& z); //Handle the specific corner case.
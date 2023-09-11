#include "Colission.h"
#include "EntityFramework.h"
#include <cmath>

/// <summary>
/// Calculates the closest distance of two circles.
/// </summary>
/// <param name="pos1X">CenterPointX</param>
/// <param name="pos1Y">CenterPointY</param>
/// <param name="r1">Radius</param>
/// <param name="pos2X">CenterPointX</param>
/// <param name="pos2Y">CenterPointY</param>
/// <param name="r2">Radius</param>
/// <returns> return < 0.0f means hit, return > 0.0f means miss</returns>
float CircularCircular(float& pos1X, float& pos1Y, float& r1, float& pos2X, float& pos2Y, float& r2)
{
	//Calculate distance between centerpoints
	float distX = pos1X - pos2X;
	float distY = pos1Y - pos2Y;
	float distance = std::powf((distX * distX + distY * distY), 0.5f);
	//Circles overlap if distance is less than sum of both radi.
	float hitDistance = distance - (r1 + r2);
	//Hit when negative, 
	return hitDistance;
}

struct CircularConvexReturn
{
	bool hit;
	int line;//0 means corners 0, 1. 1 means 1, 2. 2 means 2, 3 ir 2, 0 depending on amount of corners
	float convexAngleOfAttackX, convexAngleOfAttackY;
};

CircularConvexReturn CircularConvex(float& pos1X, float& pos1Y, float& r1/*, ConvexComponent*/)
{
	CircularConvexReturn toReturn;
	toReturn.hit = false;
	toReturn.line = 0;
	//Check if circle is behind each line and its normal respective to convex centerpoint
	//For each line
	//	Calculate line from corners an amount of corners
	//	Calculate direction of normal from line and centerpoint
	//	Check if circle center - radius in direction of normal is behind line
	//	If one line does not have circle behind, no hit
	//		Break;
	//	If all lines have circle behind, hit
	//		When hit, Find which line is closest to circle
	//		Calculate angle of attack from that line's normal
	//		Edit toReturn with new data, hit = true, line = closest line, angleOfAttack is lines normal.
	return toReturn;
}

int PositionGeometryIndependent(float posX, float posY /*GeometryIndependent Component*/)
{
	//Offset posX and posY with GI offset
	//Translate position into pixels on texture
	//Check int values for type
	//Return type
}

void CollisionEvent(Entity& entity, Entity& entity2)
{
	// find on collision components in each entity

	//extract positions from each entity 
	//find last positions from each entitiy
	// this gives us the tools to calculate the angle of attack

	//There are three different kinds of colission in this game:
	//Circular to circular: Player and Enemy hurboxes and hitboxes.
	//Circular to convex: Hurtboxes to walls(stage) or Boss hitboxes to player hurtbox
	//Point to Geometry Independent: Players and Entities to stage and static hazards

	//Here is the overview of the math to calculate if a collision occurs
	//Circular to circular: Check distance with radius in mind
	//Circular to convex: Use normals in convex to check if a point in circle is behind all lines.
	//Point to Geometry Independent: Translate point to position on pixel in texture, check value for type.

	//Attack angle is calculated when colliding:
	//Circle, centerpoint to centerpoint
	//Convex, Look at normal of line that collided by colliding with line between centerpoints.
	//Geometry independent: No need for attack angle, always upwards.

	// calls on OnCollision
}



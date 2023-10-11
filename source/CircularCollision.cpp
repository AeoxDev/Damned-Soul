#include "Backend\CircularCollision.h"
#include "Backend\Collision.h"
#include "Registry.h"
#include "Components.h"
#include <cmath>
#define NO false

bool IsCircularCollision(EntityID& entity1, EntityID& entity2, int circleID1, int circleID2)
{
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(entity2);
	float pos1x = 0.0f;
	float pos1z = 0.0f;
	float pos2x = 0.0f;
	float pos2z = 0.0f;
	if (transform1 != nullptr)
	{
		pos1x = transform1->positionX;
		pos1z = transform1->positionZ;
	}
	if (transform2 != nullptr)
	{
		pos2x = transform2->positionX;
		pos2z = transform2->positionZ;
	}
	// get a hold of hitbox components from entity
	HitboxComponent* circle1 = registry.GetComponent<HitboxComponent>(entity1);
	HitboxComponent* circle2 = registry.GetComponent<HitboxComponent>(entity2);

	// get a hold of both the circles' bit flags to check if they can collide or not
	if (!circle1||!circle2)
	{
		return false;
	} // add more check to more flags later, try with this first to see if the math works

	//Bitmask circle1 is to circle2 hit
	unsigned short iSmask =		0b0000000001111110;// is mask
	unsigned short hitMask =	0b0001111110000000;
	unsigned short* r1 = (unsigned short*)&circle1->circularFlags[circleID1];
	unsigned short* r2 = (unsigned short*)&circle2->circularFlags[circleID2];

	bool iShit1 = (( *r1 & iSmask) << 6) & ( *r2 & hitMask);//circle1 hit circle2. is hit
	bool iShit2 = (( *r2 & iSmask) << 6) & ( *r1 & hitMask);//circle2 hit circle1. is hit
	
	//If none can interact return false
	if (!iShit1&&!iShit2)
	{
		return false;
	}
	if (circle1->circleHitbox[circleID1].offsetZ != 0.0f)
	{
		circle1->circleHitbox[circleID1].offsetZ += 0.0f;
	}
	// if not, find the hitboxes and see if they actually collide
	float offset1X = circle1->circleHitbox[circleID1].offsetX * circle1->offsetXx + circle1->circleHitbox[circleID1].offsetZ * circle1->offsetXz;
	float offset2X = circle2->circleHitbox[circleID2].offsetX * circle2->offsetXx + circle2->circleHitbox[circleID2].offsetZ * circle2->offsetXz;
	float offset1Z = circle1->circleHitbox[circleID1].offsetZ * circle1->offsetZz + circle1->circleHitbox[circleID1].offsetX * circle1->offsetZx;
	float offset2Z = circle2->circleHitbox[circleID2].offsetZ * circle2->offsetZz + circle2->circleHitbox[circleID2].offsetX * circle2->offsetZx;
	float dx = (pos1x + offset1X) - (pos2x + offset2X); 
	float dz = (pos1z + offset1Z) - (pos2z + offset2Z); 
	float distance = std::sqrtf(dx * dx + dz * dz);
	bool hit = distance <= (circle1->circleHitbox[circleID1].radius + circle2->circleHitbox[circleID2].radius);
	//Use onCollission function for first and second respectively
	OnCollisionParameters params = {};
	float normalX = dx / distance;
	float normalZ = dz / distance;

	if (iShit2&&hit)
	{
		params.entity1 = entity1;
		params.entity2 = entity2;
		params.hitboxID1 = circleID1;
		params.hitboxID2 = circleID2;
		params.normal1X = -normalX;
		params.normal1Z = -normalZ;
		params.normal2X = normalX;
		params.normal2Z = normalZ;
		params.pos1X = offset1X + pos1x;
		params.pos1Z = offset1Z + pos1z;
		params.pos2X = offset2X + pos2x;
		params.pos2Z = offset2Z + pos2z;
		//If circle1 collides with circle2 and has a function:
		//!!!Change normal to make use of lastPos to ensure correct side of circle during collision
		circle1->onCircleCollision[circleID1].CollisionFunction(params);
	}
	if (iShit1&&hit)
	{
		params.entity1 = entity2;
		params.entity2 = entity1;
		params.hitboxID1 = circleID2;
		params.hitboxID2 = circleID1;
		params.normal1X = normalX;
		params.normal1Z = normalZ;
		params.normal2X = -normalX;
		params.normal2Z = -normalZ;
		params.pos2X = offset1X + pos1x;
		params.pos2Z = offset1Z + pos1z;
		params.pos1X = offset2X + pos2x;
		params.pos1Z = offset2Z + pos2z;
		//!!!Change normal to make use of lastPos to ensure correct side of circle during collision
		circle2->onCircleCollision[circleID2].CollisionFunction(params);
	}

	return hit;

	// return true/false
}

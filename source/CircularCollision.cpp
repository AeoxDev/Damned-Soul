#include "Backend\CircularCollision.h"
#include "Backend\Collision.h"
#include "Registry.h"
#include "Components.h"
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

	// if not, find the hitboxes and see if they actually collide
	float dx = (pos1x + circle1->circleHitbox[circleID1].offsetX) - (pos2x + circle2->circleHitbox[circleID2].offsetX); //change offsetX/Y to position x/y later!!!!!!!!!!!!!!!!!!!!!!!!
	float dy = (pos1z + circle1->circleHitbox[circleID1].offsetZ) - (pos2z + circle2->circleHitbox[circleID2].offsetZ); //change offsetX/Y to position x/y later!!!!!!!!!!!!!!!!!!!!!!!!
	float distance = std::sqrt(dx * dx + dy * dy);
	bool hit = distance <= (circle1->circleHitbox[circleID1].radius + circle2->circleHitbox[circleID2].radius);
	//Use onCollission function for first and second respectively
	OnCollisionParameters params = {};

	if (iShit1&&hit)
	{
		params.entity1 = entity1;
		params.entity2 = entity2;
		params.hitboxID1 = circleID1;
		params.hitboxID2 = circleID2;
		params.normal1X = -dx;
		params.normal1Z = -dy;
		params.normal2X = dx;
		params.normal2Z = dy;
		//If circle1 collides with circle2 and has a function:
		//!!!Change normal to make use of lastPos to ensure correct side of circle during collision
		circle1->onCircleCollision[circleID1].CollisionFunction(params);
	}
	if (iShit2&&hit)
	{
		params.entity1 = entity2;
		params.entity2 = entity1;
		params.hitboxID1 = circleID2;
		params.hitboxID2 = circleID1;
		params.normal1X = dx;
		params.normal1Z = dy;
		params.normal2X = -dx;
		params.normal2Z = -dy;
		//!!!Change normal to make use of lastPos to ensure correct side of circle during collision
		circle2->onCircleCollision[circleID2].CollisionFunction(params);
	}

	return hit;

	// return true/false
}

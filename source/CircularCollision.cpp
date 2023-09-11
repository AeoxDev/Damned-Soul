#include "Backend\CircularCollision.h"
#include "Backend\Collision.h"


bool IsCircularCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circleID1, int circleID2)
{
	// get a hold of hitbox components from entity
	HitboxComponent* circle1 = registry.GetComponent<HitboxComponent>(entity1);
	HitboxComponent* circle2 = registry.GetComponent<HitboxComponent>(entity2);

	// get a hold of both the circles' bit flags to check if they can collide or not
	if (!circle1||!circle2||!circle1->circularFlags[circleID1].active||!circle2->circularFlags[circleID2].active)
	{
		return false;
	} // add more check to more flags later, try with this first to see if the math works

	//Bitmask circle1 is to circle2 hit
	unsigned short iSmask = 0b0000000001111110;// is mask
	unsigned short hitMask = 0b0001111110000000;
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
	float dx = circle1->circleHitbox[circleID1].offsetX - circle2->circleHitbox[circleID2].offsetX; //change offsetX/Y to position x/y later!!!!!!!!!!!!!!!!!!!!!!!!
	float dy = circle1->circleHitbox[circleID1].offsetZ - circle2->circleHitbox[circleID2].offsetZ; //change offsetX/Y to position x/y later!!!!!!!!!!!!!!!!!!!!!!!!
	float distance = std::sqrt(dx * dx + dy * dy);
	bool hit = distance <= (circle1->circleHitbox[circleID1].radius + circle2->circleHitbox[circleID2].radius);
	//Use onCollission function for first and second respectively
	if (iShit1&&hit)
	{
		//If circle1 collides with circle2 and has a function:
		//Calculate angle of attack
		//circle1->onCircleCollision[circleID1].CollisionFunction(registry, entity1, entity2, )

	}
	if (iShit2&&hit)
	{
		//Calculate angle of attack
	}

	return hit;

	// return true/false
}

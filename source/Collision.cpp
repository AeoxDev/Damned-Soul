#include "Backend/Collision.h"
#include "Backend/ConvexCollision.h"
#include "Backend/CircularCollision.h"
#include "Backend/ProximityCollision.h"
#include "EntityFramework.h"
#include "Registry.h"
#include <cmath>
#include <string>
#include "Systems\Systems.h"

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
//float CircularCircular(float& pos1X, float& pos1Z, float& r1, float& pos2X, float& pos2Z, float& r2)
//{
//	//Calculate distance between centerpoints
//	float distX = pos1X - pos2X;
//	float distY = pos1Y - pos2Y;
//	float distance = std::powf((distX * distX + distY * distY), 0.5f);
//	//Circles overlap if distance is less than sum of both radi.
//	float hitDistance = distance - (r1 + r2);
//	//Hit when negative, 
//	return hitDistance;
//}

struct CircularConvexReturn
{
	bool hit;
	int line;//0 means corners 0, 1. 1 means 1, 2. 2 means 2, 3 ir 2, 0 depending on amount of corners
	float convexAngleOfAttackX, convexAngleOfAttackY;
};

CircularConvexReturn CircularConvex(float& pos1X, float& pos1Z, float& r1/*, ConvexComponent*/)
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

int PositionGeometryIndependent(float posX, float posZ /*GeometryIndependent Component*/)
{
	//Offset posX and posY with GI offset
	//Translate position into pixels on texture
	//Check int values for type
	//Return type
	return 0;
}

//void CollisionEvent(Entity& entity, Entity& entity2)
//{
//	// find on collision components in each entity
//
//	//extract positions from each entity 
//	//find last positions from each entitiy
//	// this gives us the tools to calculate the angle of attack
//
//	//There are three different kinds of colission in this game:
//	//Circular to circular: Player and Enemy hurboxes and hitboxes.
//	//Circular to convex: Hurtboxes to walls(stage) or Boss hitboxes to player hurtbox
//	//Point to Geometry Independent: Players and Entities to stage and static hazards
//
//	//Here is the overview of the math to calculate if a collision occurs
//	//Circular to circular: Check distance with radius in mind
//	//Circular to convex: Use normals in convex to check if a point in circle is behind all lines.
//	//Point to Geometry Independent: Translate point to position on pixel in texture, check value for type.
//
//	//Attack angle is calculated when colliding:
//	//Circle, centerpoint to centerpoint
//	//Convex, Look at normal of line that collided by colliding with line between centerpoints.
//	//Geometry independent: No need for attack angle, always upwards.
//
//	// calls on OnCollision
//}

//Returns the bit that is available, 
//Returns 0 if no bits are available
int FindAvailableSlot(unsigned& bits)
{
	unsigned mask = 0b1;//0b00000000 00000000 00000000 00000001
	unsigned size = 0;
	//Look if first bit is 0;
	while (bits & mask)
	{
		if (size >= SAME_TYPE_HITBOX_LIMIT)
		{
			//Max size hit,
			return -1;
		}
		//Check next bit
		mask = mask << 1;
		++size;
	}
	bits |= mask;//Set the bit to 1
	return size;
}

void ResetCollisionVariables()
{
	for (auto entity : View<HitboxComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		HitboxComponent* hitboxes = registry.GetComponent<HitboxComponent>(entity);
		if (hitboxes == nullptr)
		{
			continue;
		}
		hitboxes->nrMoveableCollisions = MOVEABLE_COLLISIONS_PER_FRAME;
	}
}

void HandleDamageCollision()
{
	//Stuff happens here. WOW!

	for (auto entity : View<HitboxComponent>(registry)) //Access the first entity
	{
		HitboxComponent* firstHitbox = registry.GetComponent<HitboxComponent>(entity);

		for (int i = 0; i < SAME_TYPE_HITBOX_LIMIT; i++)
		{
			//Add CanDealDamageCheck
			for (auto entity2 : View<HitboxComponent>(registry)) //Access the second entity
			{
				if (entity.index == entity2.index) //Check that it's not the same as first entity
				{
					continue;
				}

				HitboxComponent* secondHitbox = registry.GetComponent<HitboxComponent>(entity2);

				for (int j = 0; j < SAME_TYPE_HITBOX_LIMIT; j++)
				{
					//Add CanTakeDamageCheck
				}
			}
		}
	}
}

void HandleStaticCollision( )
{
	//Look at picture. Amaziung!
}

void HandleMoveableCollision( )//Reggie Strie
{
	HandleProximityCollision();
	for (auto entity : View<HitboxComponent>(registry)) //Access the first entity
	{
		HitboxComponent* firstHitbox = registry.GetComponent<HitboxComponent>(entity);

		if (firstHitbox->nrMoveableCollisions < 1) //If it can't move anymore this frame
		{
			continue;
		}
		for (int i = 0; i < SAME_TYPE_HITBOX_LIMIT; i++)
		{
			for (auto entity2 : View<HitboxComponent>(registry)) //Access the second entity
			{
				if (entity.index == entity2.index) //Check that it's not the same as first entity
				{
					continue;
				}

				HitboxComponent* secondHitbox = registry.GetComponent<HitboxComponent>(entity2);

				for (int j = 0; j < SAME_TYPE_HITBOX_LIMIT; j++)
				{
					if (firstHitbox->circularFlags[i].active && secondHitbox->circularFlags[j].active)
					{
						//Both are circular, do circle to circle.
						bool hit = IsCircularCollision(entity, entity2, i, j);
					}
					if (firstHitbox->convexFlags[i].active && secondHitbox->convexFlags[j].active)
					{
						//Both are convex, do convex to convex.
						bool hit = IsConvexCollision(entity, entity2, i, j);
					}
					if (firstHitbox->circularFlags[i].active && firstHitbox->convexFlags[j].active)
					{
						//One is convex, other one is circular.
						bool hit = IsCircularConvexCollision(entity, entity2, i, j); //Could add a check for which is convex/circular if the parameter order matters.
					}
					if (firstHitbox->convexFlags[i].active && firstHitbox->circularFlags[j].active)
					{
						//One is convex, other one is circular.
						bool hit = IsConvexCircularCollision(entity, entity2, i, j); //Could add a check for which is convex/circular if the parameter order matters.
					}
					/*else
					{
						continue;
					}*/
				}
			}
		}
	}
}

void HandleProximityCollision()
{
	for (auto entity : View<HitboxComponent>(registry)) //Access an entity
	{
		float distance = 1000.0f;
		float x, z;
		int index = 0;
		EntityID closestWall = { 0 };
		closestWall.index = -1;
		HitboxComponent* entityHitbox = registry.GetComponent<HitboxComponent>(entity);

		if (entityHitbox->circularFlags[0].hitWall && entityHitbox->circularFlags[0].active) //Check if the entity can hit the wall and is circular
		{
			for (auto entity2 : View<ProximityHitboxComponent>(registry)) //Access the wall
			{
				ProximityHitboxComponent* wallHitbox = registry.GetComponent<ProximityHitboxComponent>(entity2);
				if (wallHitbox == nullptr)
				{
					continue;
				}
				for (int i = 0; i < (int)(wallHitbox->pointList.size()); i++) //Loop through all points in the wallHitbox
				{
					float currentDistance = sqrt(float((pow(entityHitbox->circleHitbox[0].offsetX - wallHitbox->pointList[i].x, 2)) + (pow(entityHitbox->circleHitbox[0].offsetZ - wallHitbox->pointList[i].z, 2))));
					
					if (distance > currentDistance) //Check if closer to the new point checked
					{
						distance = currentDistance;
						index = i;
						closestWall = entity2;
						x = entityHitbox->circleHitbox[0].offsetX;
						z = entityHitbox->circleHitbox[0].offsetZ;
					}
				}
			}
		}
		else if (entityHitbox->convexFlags[0].hitWall && entityHitbox->convexFlags[0].active) //Check if the entity can hit the wall and is convex
		{
			for (auto entity2 : View<ProximityHitboxComponent>(registry)) //Access the wall
			{
				ProximityHitboxComponent* wallHitbox = registry.GetComponent<ProximityHitboxComponent>(entity2);

				for (int i = 0; i < (int)(wallHitbox->pointList.size()); i++) //Loop through all points in the wallHitbox
				{
					float currentDistance = sqrt(float((pow(entityHitbox->convexHitbox[0].centerX - wallHitbox->pointList[i].x, 2)) + (pow(entityHitbox->convexHitbox[0].centerZ - wallHitbox->pointList[i].z, 2))));
					
					if (distance > currentDistance) //Check if closer to the new point checked
					{
						distance = currentDistance;
						index = i;
						closestWall = entity2;
						x = entityHitbox->convexHitbox[0].centerX;
						z = entityHitbox->convexHitbox[0].centerZ;
					}
				}
			}
		}

		if (closestWall.index != -1) //If an entity has been assigned as the closestWall
		{
			ProximityCorrection(closestWall, index, x, z);
		}
	}
}

HitboxComponent::HitboxComponent()
{
	this->usedCirclesHitboxes = 0;
	this->usedConvexHitboxes = 0;
	long long* flags = (long long*)this->circularFlags;
	*flags = 0;
	this->nrMoveableCollisions = 0;
	flags = (long long*)this->convexFlags;
	*flags = 0;
	//No need to zero the other values as they won't be used until mode is set.
}

void CollisionFlags::ResetToActive()
{
	unsigned short* r = (unsigned short*)this;
	*r = 0;
	this->active = 1;
	
}

ProximityHitboxComponent::ProximityHitboxComponent()
{
	this->clockwise = 1;
}

void UpdatePhysics()
{
	ResetCollisionVariables();
	HandleMoveableCollision();
	HandleDamageCollision();
	HandleStaticCollision();
}

bool CollisionSystem::Update()
{
	UpdatePhysics();
	return true;
}
#pragma once
#define CIRCLE_COMPONENT_INDEX 2 //change to correct number later
#define SAME_TYPE_HITBOX_LIMIT 4 //Limit for same type of hitboxes
#define CONVEX_CORNER_LIMIT 8 //Maximum amount of corners allowed per Convex shape
#define MAP_DIM 512*512
#define MOVEABLE_COLLISIONS_PER_FRAME 1
#include "EntityFramework.h"
#include "PoolUtility.hpp"


struct OnCollisionParameters
{
	Registry registry; 
	EntityID entity1; 
	EntityID entity2; 
	int hitboxID1;
	int hitboxID2;
	float normal1X;
	float normal1Z;
	float normal2X;
	float normal2Z;
};

struct OnCollision
{
	void(*CollisionFunction)(OnCollisionParameters& params);
};

struct CollisionFlags
{
	unsigned short active				: 1;
	//This is of type
	unsigned short isStage				: 1;//Collide with stage
	unsigned short isWall				: 1;//Collide with walls and static objects
	unsigned short isPlayer				: 1;//Collide with player
	unsigned short isEnemy				: 1;//Collide with enemy
	unsigned short isStaticHazard		: 1;//Collide with static hazards
	unsigned short isDynamicHazard		: 1;//Collide with dynamic

	//This collides with other types
	unsigned short hitStage				: 1;//Collide with stage
	unsigned short hitWall				: 1;//Collide with walls and static objects
	unsigned short hitPlayer			: 1;//Collide with player
	unsigned short hitEnemy				: 1;//Collide with enemy
	unsigned short hitStaticHazard		: 1;//Collide with static hazards
	unsigned short hitDynamicHazard		: 1;//Collide with dynamic

	unsigned short isMoveable			: 1;//Deterrmine wether or not it is possible to move this object in collision
	unsigned short canTakeDamage		: 1;//Determines if hitbox can take damage
	unsigned short canDealDamage		: 1;//Determines if hitbox can deal damage

	void ResetToActive();
};


struct CircleColliderComponent
{
	//Might need array of circles instead
	float radius, offsetX, offsetZ;
};

struct ConvexColliderComponent
{
	//might need array of shapes instead
	float centerX, centerZ;
	float boundingRadius;//Circle to reduce calculations for performance
	int cornerAmount;
	float cornerX[CONVEX_CORNER_LIMIT], cornerZ[CONVEX_CORNER_LIMIT];
	float normalX[CONVEX_CORNER_LIMIT], normalZ[CONVEX_CORNER_LIMIT];
};

struct GeometryIndependentColliderComponent
{
	unsigned char pixelMap[MAP_DIM]; //#define 512*512 as size? or rather just 512 as dimension
	float offsetX, offsetZ;
};

struct HitboxComponent
{
	int nrMoveableCollisions;

	unsigned usedCirclesHitboxes;//This is a collection of bits that indicate which are used or not
	CollisionFlags circularFlags[SAME_TYPE_HITBOX_LIMIT] = {0};
	OnCollision onCircleCollision[SAME_TYPE_HITBOX_LIMIT] = { 0 };//What happens when this hitbox collides with something
	CircleColliderComponent circleHitbox[SAME_TYPE_HITBOX_LIMIT] = { 0 };

	unsigned usedConvexHitboxes;//This is a collection of bits that indicate which are used or not
	CollisionFlags convexFlags[SAME_TYPE_HITBOX_LIMIT] = { 0 };
	OnCollision onConvexCollision[SAME_TYPE_HITBOX_LIMIT] = { 0 };//What happens when this hitbox collides with something
	ConvexColliderComponent convexHitbox[SAME_TYPE_HITBOX_LIMIT] = { 0 };

	HitboxComponent();
	
};

struct ProximityPoint
{
	int index;
	float x;
	float z;
};

struct ProximityHitboxComponent
{
	int clockwise = 1; //Clockwise or counter-clockwise
	PoolVector<ProximityPoints> pointList = { 0 }; //List of all points in the hitbox

	ProximityHitboxComponent();
};

int FindAvailableSlot(unsigned& bits);

/// <summary>
/// Loops through all entities with a hitboxComponent and calculates collision.
/// </summary>
/// <param name="registry"></param>
void ResetCollisionVariables(Registry& registry);
void HandleMoveableCollision(Registry& registry);//No clipping
void HandleProximityCollision(Registry& registry); //No proximity clipping
void HandleDamageCollision(Registry& registry);//Hitboxes for attacks and dynamic hazards
void HandleStaticCollision(Registry& registry);//Stage and static Collisions

//struct onCollision
//{
//	void(*CollisionFunction)(Entity& entity, Entity& entity2, float angleOfAttack1X, float angleOfAttack1Y, float angleOfAttack2X, float angleOfAttack2Y);
//};
//void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision
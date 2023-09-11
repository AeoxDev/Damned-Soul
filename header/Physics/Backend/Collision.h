#pragma once
#define CIRCLE_COMPONENT_INDEX 2 //change to correct number later
#define SAME_TYPE_HITBOX_LIMIT 4 //Limit for same type of hitboxes
#define CONVEX_CORNER_LIMIT 8 //Maximum amount of corners allowed per Convex shape
#define MAP_DIM 512*512
#include "EntityFramework.h"

struct OnCollision
{
	void(*CollisionFunction)(Registry& registry, EntityID& entity1, EntityID& entity2, float angleOfAttack1X, float angleOfAttack1Z, float angleOfAttack2X, float angleOfAttack2Z);
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

	unsigned short padding1 : 1;
	unsigned short padding2 : 1;
	unsigned short padding3 : 1;

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
	float cornerX[CONVEX_CORNER_LIMIT], cornerY[CONVEX_CORNER_LIMIT];
};

struct GeometryIndependentColliderComponent
{
	unsigned char pixelMap[MAP_DIM]; //#define 512*512 as size? or rather just 512 as dimension
	float offsetX, offsetZ;
};

struct HitboxComponent
{
	unsigned activeCirclesHitboxes;//This is a collection of bits that indicate which are active or not
	CollisionFlags circularFlags[SAME_TYPE_HITBOX_LIMIT];
	OnCollision onCircleCollision[SAME_TYPE_HITBOX_LIMIT];//What happens when this hitbox collides with something
	CircleColliderComponent circleHitbox[SAME_TYPE_HITBOX_LIMIT];

	unsigned activeConvexHitboxes;//This is a collection of bits that indicate which are active or not
	CollisionFlags convexFlags[SAME_TYPE_HITBOX_LIMIT];
	OnCollision onConvexCollision[SAME_TYPE_HITBOX_LIMIT];//What happens when this hitbox collides with something
	ConvexColliderComponent convexHitbox[SAME_TYPE_HITBOX_LIMIT];

	HitboxComponent();
	
};

int FindAvailableSlot(unsigned& bits);


//struct onCollision
//{
//	void(*CollisionFunction)(Entity& entity, Entity& entity2, float angleOfAttack1X, float angleOfAttack1Y, float angleOfAttack2X, float angleOfAttack2Y);
//};
//void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision
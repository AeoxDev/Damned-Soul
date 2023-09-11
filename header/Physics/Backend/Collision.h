#pragma once
#define CIRCLE_COMPONENT_INDEX 2 //change to correct number later
#define SAME_TYPE_HITBOX_LIMIT 4 //Limit for same type of hitboxes
#define CONVEX_CORNER_LIMIT 8 //Maximum amount of corners allowed per Convex shape
#define MAP_DIM 512*512




struct CollisionFlags
{
	unsigned short active				: 1;
	unsigned short stage				: 1;//Collide with stage
	unsigned short wall					: 1;//Collide with walls and static objects
	unsigned short player				: 1;//Collide with player
	unsigned short enemy				: 1;//Collide with enemy
	unsigned short staticHazard			: 1;//Collide with static hazards
	unsigned short dynamicHazard		: 1;//Collide with dynamic
	unsigned short collideWithSelf		: 1;//This is padding until needed for bug fixes

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
	CircleColliderComponent circleHitbox[SAME_TYPE_HITBOX_LIMIT];

	unsigned activeConvexHitboxes;//This is a collection of bits that indicate which are active or not
	CollisionFlags convexFlags[SAME_TYPE_HITBOX_LIMIT];
	ConvexColliderComponent convexHitbox[SAME_TYPE_HITBOX_LIMIT];

	HitboxComponent();
	
};

int FindAvailableSlot(unsigned& bits);


//struct onCollision
//{
//	void(*CollisionFunction)(Entity& entity, Entity& entity2, float angleOfAttack1X, float angleOfAttack1Y, float angleOfAttack2X, float angleOfAttack2Y);
//};
//void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision
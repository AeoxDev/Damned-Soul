#pragma once
#define CIRCLE_COMPONENT_INDEX 2 //change to correct number later
#define SAME_TYPE_HITBOX_LIMIT 4 //Limit for same type of hitboxes
#define CONVEX_CORNER_LIMIT 8 //Maximum amount of corners allowed per Convex shape

struct CollisionFlags
{
	unsigned stage				: 1;//Collide with stage
	unsigned wall				: 1;//Collide with walls and static objects
	unsigned player				: 1;//Collide with player
	unsigned enemy				: 1;//Collide with enemy
	unsigned staticHazard		: 1;//Collide with static hazards
	unsigned dynamicHazard		: 1;//Collide with dynamic
	unsigned collideWithSelf	: 1;
	unsigned padding			: 1;
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
	uint8_t pixelMap[512 * 512]; //#define 512*512 as size? or rather just 512 as dimension
	float offsetX, offsetZ;
};

struct CollisionComponent
{
	int activeCirclesHitboxes;
	CollisionFlags circularFlags[SAME_TYPE_HITBOX_LIMIT];
	CircleColliderComponent circleHitbox[SAME_TYPE_HITBOX_LIMIT];

	int activeConvexHitboxes;
	CollisionFlags convexFlags[SAME_TYPE_HITBOX_LIMIT];
	ConvexColliderComponent circleHitbox[SAME_TYPE_HITBOX_LIMIT];
};
//struct onCollision
//{
//	void(*CollisionFunction)(Entity& entity, Entity& entity2, float angleOfAttack1X, float angleOfAttack1Y, float angleOfAttack2X, float angleOfAttack2Y);
//};
//void CollisionEvent(Entity& entity, Entity& entity2); //calculates angle of attack and sends it to onCollision
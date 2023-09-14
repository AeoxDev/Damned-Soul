#include "Backend\ConvexCollision.h"
#include "Backend\Collision.h"

bool IsCircularConvexCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circleID, int convexID)
{
	// get a hold of hitbox components from entity
	HitboxComponent* circle = registry.GetComponent<HitboxComponent>(entity1);
	HitboxComponent* convex = registry.GetComponent<HitboxComponent>(entity2);

	// get a hold of both the circles' bit flags to check if they can collide or not
	if (!circle || !convex)
	{
		return false;
	} // add more check to more flags later, try with this first to see if the math works

	//Bitmask circle1 is to circle2 hit
	unsigned short iSmask = 0b0000000001111110;// is mask
	unsigned short hitMask = 0b0001111110000000;
	unsigned short* r1 = (unsigned short*)&circle->circularFlags[circleID];
	unsigned short* r2 = (unsigned short*)&convex->circularFlags[convexID];

	bool iShit1 = ((*r1 & iSmask) << 6) & (*r2 & hitMask);//circle1 hit circle2. is hit
	bool iShit2 = ((*r2 & iSmask) << 6) & (*r1 & hitMask);//circle2 hit circle1. is hit

	//If none can interact return false
	if (!iShit1 && !iShit2)
	{
		return false;
	}
	float cornerToCircleX, cornerToCircleZ = 0.f;
	float scalarDist = 0.f;
	int normalHit = -1;
	//Check if circle if behind all of the lines of the convex shape
	for (size_t i = 0; i < convex->convexHitbox[convexID].cornerAmount; i++)
	{
		//Get line
		cornerToCircleX = circle->circleHitbox[i].offsetX - convex->convexHitbox[convexID].cornerX[i];
		cornerToCircleZ = circle->circleHitbox[i].offsetZ - convex->convexHitbox[convexID].cornerZ[i];

		//Do scalar with normal
		scalarDist = (cornerToCircleX * convex->convexHitbox[convexID].normalX[i]) + (cornerToCircleZ * convex->convexHitbox[convexID].normalZ[i]);

		//Check for radius
		scalarDist = scalarDist - circle->circleHitbox[i].radius;

		//If still outside, give up.
		if (scalarDist > 0.0f)
		{
			return false;
		}
	}
	//Use onCollission function for first and second respectively
	OnCollisionParameters params = {};
	params.registry = registry;//Reggie stiel
	float convexToCircleX, convexToCircleZ;
	convexToCircleX = circle->circleHitbox[circleID].offsetX - convex->convexHitbox[convexID].centerX;
	convexToCircleZ = circle->circleHitbox[circleID].offsetZ - convex->convexHitbox[convexID].centerZ;
	if (iShit1)
	{
		params.entity1 = entity1;
		params.entity2 = entity2;
		params.hitboxID1 = circleID;
		params.hitboxID2 = convexID;
		params.normal1X = -convexToCircleX;
		params.normal1Z = -convexToCircleZ;
		params.normal2X = convexToCircleX;
		params.normal2Z = convexToCircleZ;
		//If circle1 collides with circle2 and has a function:
		//!!!Change normal to make use of lastPos to ensure correct side of circle during collision
		circle->onCircleCollision[circleID].CollisionFunction(params);
	}
	if (iShit2)
	{
		params.entity1 = entity2;
		params.entity2 = entity1;
		params.hitboxID1 = convexID;
		params.hitboxID2 = circleID;
		params.normal1X = convexToCircleX;
		params.normal1Z = convexToCircleZ;
		params.normal2X = -convexToCircleX;
		params.normal2Z = -convexToCircleZ;
		//!!!Change normal to make use of lastPos to ensure correct side of circle during collision
		convex->onCircleCollision[convexID].CollisionFunction(params);
	}

	return true;

}

bool IsConvexCircularCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int convex, int circle)
{
	return IsCircularConvexCollision(registry, entity2, entity1, circle, convex);
}

//Vector4 LineLineIntersection(Vector2 l1p1, Vector2 l1p2, Vector2 l2p1, Vector2 l2p2)
//{
//	//https://github.com/OneLoneCoder/olcPixelGameEngine/blob/master/Videos/OneLoneCoder_PGE_PolygonCollisions1.cpp
//	float denominator = (l2p2.X - l2p1.X) * (l1p1.Y - l1p2.Y) - (l1p1.X - l1p2.X) * (l2p2.Y - l2p1.Y);
//	float t1 = ((l2p1.Y - l2p2.Y) * (l1p1.X - l2p1.X) + (l2p2.X - l2p1.X) * (l1p1.Y - l2p1.Y)) / denominator;
//	float t2 = ((l1p1.Y - l1p2.Y) * (l1p1.X - l2p1.X) + (l1p2.X - l1p1.X) * (l1p1.Y - l2p1.Y)) / denominator;
//	if (t1 >= 0.0f && t1 < 1.0f && t2 >= 0.0f && t2 < 1.0f)
//	{
//		Vector2 line1 = new Vector2((1.0f - t1) * (l1p2.X - l1p1.X), (1.0f - t1) * (l1p2.Y - l1p1.Y));
//		Vector2 line2 = new Vector2((1.0f - t2) * (l2p2.X - l2p1.X), (1.0f - t2) * (l2p2.Y - l2p1.Y));
//		return new Vector4(line1.X, line1.Y, line2.X, line2.Y);
//	}
//	return new Vector4(0, 0, 0, 0);
//}

bool IsConvexCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int circle1, int circle2)
{
	//
	return false;
}

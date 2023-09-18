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

// //Kolla denna polygons hörnor
//for (int i = 0; i < totalVertices; i++)
//{
//	for (int j = 0; j < collider.totalVertices; j++)
//	{
//		result = LineLineIntersection(center, vertices[i].GetPosition(), collider.lines[j].point1, collider.lines[j].point2);
//		if (result.X != 0 || result.Y != 0)
//		{
//			normal = collider.lines[j].normal;
//			Translate(-1 * new Vector2(result.X, result.Y));
//			//collider.Translate(-1 * new Vector2(result.Z, result.W)); 
//			//UpdateAll();
//			collision = true;
//			InitiateCollision(collider, new Vector2(result.Z, result.W), j, normal, i);
//			return collision;
//		}
//	}
//}

struct ConvexReturnData
{
	bool hit;
	float line1X, line1Z, line2X, line2Z;
};

/// <summary>
/// 
/// </summary>
/// <param name="line1Corner1X">= l1c1x</param>
/// <param name="line1Corner1Z">= l1c1z</param>
/// <param name="line1Corner2X">= l1c2x</param>
/// <param name="line1Corner2Z">= l1c2z</param>
/// <param name="line2Corner1X">= l2c1x</param>
/// <param name="line2Corner1Z">= l2c1z</param>
/// <param name="line2Corner2X">= l2c2x</param>
/// <param name="line2Corner2Z">= l2c2z</param>
/// <returns></returns>
ConvexReturnData LineToLineIntersection(float l1c1x, float l1c1z, float l1c2x, float l1c2z, float l2c1x, float l2c1z, float l2c2x, float l2c2z)
{
	//Get denominator to get where on line the intersection is from 0 to 1
	float denominator = (l2c2x - l2c1x) * (l1c1z - l1c2z) - (l1c1x - l1c2x) * (l2c2z - l2c1z);

	//Get where on the lines the intersection happens, use denominator to get to 0 to 1 if on line
	float whereOnLine1 = ((l2c1z - l2c2z) * (l1c1x - l2c1x) + (l2c2x - l2c1x) * (l1c1z - l2c1z)) / denominator;
	float whereOnLine2 = ((l1c1z - l1c2z) * (l1c1x - l2c1x) + (l1c2x - l1c1x) * (l1c1z - l2c1z)) / denominator;
	ConvexReturnData toReturn = { 0 };
	//Check if the intersection happens between the corners of the lines
	//if outside of 0 and 1 the intersection is outside of convex shape
	if (whereOnLine1 >= 0.0f && whereOnLine1 < 1.0f && whereOnLine2 >= 0.0f && whereOnLine2 < 1.0f)
	{
		toReturn.line1X = (1.0f - whereOnLine1) * (l1c2x - l1c1x);
		toReturn.line1Z = (1.0f - whereOnLine1) * (l1c2z - l1c1z);
		toReturn.line2X = (1.0f - whereOnLine2) * (l2c2x - l2c1x);
		toReturn.line2Z = (1.0f - whereOnLine2) * (l2c2z - l2c1z);
		toReturn.hit = true;
	}
	return toReturn;
}

bool IsConvexCollision(Registry& registry, EntityID& entity1, EntityID& entity2, int convexID1, int convexID2)
{
	HitboxComponent *convex1 = registry.GetComponent<HitboxComponent>(entity1);
	HitboxComponent *convex2 = registry.GetComponent<HitboxComponent>(entity2);

	// get a hold of both the circles' bit flags to check if they can collide or not
	if (!convex1 || !convex2)
	{
		return false;
	} // add more check to more flags later, try with this first to see if the math works

	//Bitmask circle1 is to circle2 hit
	unsigned short iSmask = 0b0000000001111110;// is mask
	unsigned short hitMask = 0b0001111110000000;
	unsigned short* r1 = (unsigned short*)&convex1->circularFlags[convexID1];
	unsigned short* r2 = (unsigned short*)&convex2->convexFlags[convexID2];

	bool iShit1 = ((*r1 & iSmask) << 6) & (*r2 & hitMask);//circle1 hit circle2. is hit
	bool iShit2 = ((*r2 & iSmask) << 6) & (*r1 & hitMask);//circle2 hit circle1. is hit

	//If none can interact return false
	if (!iShit1 && !iShit2)
	{
		return false;
	}

	bool collided = false;
	//First check if bounding radius reach each other
	float posX = convex1->convexHitbox[convexID1].centerX;
	float posZ = convex1->convexHitbox[convexID1].centerZ;
	float pos2X = convex2->convexHitbox[convexID2].centerX;
	float pos2Z = convex2->convexHitbox[convexID2].centerZ;

	float dx = posX - pos2X;
	float dz = posZ - pos2Z;

	//If distance from each other is greater than radi, no circular collision, skip.
	if (sqrtf(dx*dx + dz*dz) > convex1->convexHitbox[convexID1].boundingRadius + convex2->convexHitbox[convexID2].boundingRadius)
	{
		return false;
	}
	ConvexReturnData returnedData = {0};
	int whichLine = 0;
	//Now we can check which lines collide
	for (size_t i = 0; i < convex1->convexHitbox[convexID1].cornerAmount; i++)
	{
		for (size_t j = 0; j < convex2->convexHitbox[convexID2].cornerAmount; j++)
		{
			//Check from center to corner of one convex shape onto the sides of another.
			returnedData = LineToLineIntersection(convex1->convexHitbox[convexID1].centerX,
				convex1->convexHitbox[convexID1].centerZ,
				convex1->convexHitbox[convexID1].cornerX[i], convex1->convexHitbox[convexID1].cornerZ[i],
				convex2->convexHitbox[convexID2].cornerX[j], convex2->convexHitbox[convexID2].cornerZ[j],
				convex2->convexHitbox[convexID2].cornerX[(1 + j) % CONVEX_CORNER_LIMIT], convex2->convexHitbox[convexID2].cornerZ[(1 + j) % CONVEX_CORNER_LIMIT]);
			if (returnedData.hit)
			{	
				whichLine = j;
				//If we add convex hurtboxes for enemies we can add moving them correctly here.
				break;
			}
		}
		if (returnedData.hit)
		{
			//If we add convex hurtboxes for enemies we can add moving them correctly here.
			break;
		}
	}
	OnCollisionParameters params = {};
	params.registry = registry;//Reggie stiel

	if (iShit1)
	{
		params.entity1 = entity1;
		params.entity2 = entity2;
		params.hitboxID1 = convexID1;
		params.hitboxID2 = convexID2;
		params.normal1X = -convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal1Z = -convex2->convexHitbox[convexID2].normalZ[whichLine];
		params.normal2X = convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal2Z = convex2->convexHitbox[convexID2].normalZ[whichLine];
		convex1->onCircleCollision[convexID1].CollisionFunction(params);
	}
	if (iShit2)
	{
		params.entity1 = entity2;
		params.entity2 = entity1;
		params.hitboxID1 = convexID1;
		params.hitboxID2 = convexID2;
		params.normal1X = convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal1Z = convex2->convexHitbox[convexID2].normalZ[whichLine];
		params.normal2X = -convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal2Z = -convex2->convexHitbox[convexID2].normalZ[whichLine];
		convex2->onCircleCollision[convexID2].CollisionFunction(params);
	}

	return returnedData.hit;
}

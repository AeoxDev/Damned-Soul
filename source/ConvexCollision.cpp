#include "Backend\ConvexCollision.h"
#include "Backend\Collision.h"
#include "Registry.h"
#include "Components.h"

bool IsCircularConvexCollision(EntityID& entity1, EntityID& entity2, int circleID, int convexID)
{
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(entity2);
	float pos1x = 0.0f;
	float pos1z = 0.0f;
	float pos2x = 0.0f;
	float pos2z = 0.0f;
	float scaling1X = 1.0f;
	float scaling1Z = 1.0f;
	float scaling1 = 1.0f;
	float scaling2X = 1.0f;
	float scaling2Z = 1.0f;
	float scaling2 = 1.0f;
	if (transform1 != nullptr)
	{
		pos1x = transform1->positionX;
		pos1z = transform1->positionZ;
		scaling1X = transform1->scaleX;
		scaling1Z = transform1->scaleZ;
	}
	if (transform2 != nullptr)
	{
		pos2x = transform2->positionX;
		pos2z = transform2->positionZ;
		scaling2X = transform2->scaleX;
		scaling2 = scaling2X;
		scaling2Z = transform2->scaleZ;
		if (scaling2Z > scaling2)
		{
			scaling2 = scaling2Z;
		}
	}
	// get a hold of hitbox components from entity
	HitboxComponent* circle = registry.GetComponent<HitboxComponent>(entity1);
	HitboxComponent* convex = registry.GetComponent<HitboxComponent>(entity2);
	if (convex == nullptr  || circle == nullptr)
	{
		return false; 
	}
	float center2X = convex->convexHitbox[convexID].centerX * scaling2X;
	float center2Z = convex->convexHitbox[convexID].centerZ * scaling2Z;
	// get a hold of both the circles' bit flags to check if they can collide or not
	if (!circle || !convex)
	{
		return false;
	} // add more check to more flags later, try with this first to see if the math works

	//Bitmask circle1 is to circle2 hit
	unsigned short iSmask = 0b0000000001111110;// is mask
	unsigned short hitMask = 0b0001111110000000;
	unsigned short* r1 = (unsigned short*)&circle->circularFlags[circleID];
	unsigned short* r2 = (unsigned short*)&convex->convexFlags[convexID];

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
		cornerToCircleX = (pos1x + RotateOffset(scaling1 * circle->circleHitbox[circleID].offsetX, scaling1 * circle->circleHitbox[circleID].offsetZ, circle->offsetXx, circle->offsetXz)) -
			(pos2x + RotateOffset(scaling2X * convex->convexHitbox[convexID].cornerX[i], scaling2Z * convex->convexHitbox[convexID].cornerZ[i], convex->offsetXx, convex->offsetXz));
		cornerToCircleZ = (pos1z + RotateOffset(scaling1 * circle->circleHitbox[circleID].offsetZ, scaling1 * circle->circleHitbox[circleID].offsetX, circle->offsetZz, circle->offsetZx)) - 
			(pos2z +RotateOffset(scaling2Z * convex->convexHitbox[convexID].cornerZ[i], scaling2X * convex->convexHitbox[convexID].cornerX[i], convex->offsetZz, convex->offsetZx));

		//Do scalar with normal
		scalarDist = (cornerToCircleX * RotateOffset(convex->convexHitbox[convexID].normalX[i], convex->convexHitbox[convexID].normalZ[i], convex->offsetXx, convex->offsetXz)) +
			(cornerToCircleZ * RotateOffset(convex->convexHitbox[convexID].normalZ[i], convex->convexHitbox[convexID].normalX[i], convex->offsetZz, convex->offsetZx));

		//Check for radius
		scalarDist = scalarDist - circle->circleHitbox[circleID].radius*scaling1;

		//If still outside, give up.
		if (scalarDist > 0.0f)
		{
			return false;
		}
	}
	//Use onCollission function for first and second respectively
	OnCollisionParameters params = {};
	float convexToCircleX, convexToCircleZ;
	convexToCircleX = (pos1x + RotateOffset(circle->circleHitbox[circleID].offsetX, circle->circleHitbox[circleID].offsetZ, circle->offsetXx, circle->offsetXz)) -
		(pos2x + RotateOffset(scaling2X * convex->convexHitbox[convexID].centerX, scaling2Z * convex->convexHitbox[convexID].centerZ, convex->offsetXx, convex->offsetXz));
	convexToCircleZ = (pos1x + RotateOffset(circle->circleHitbox[circleID].offsetZ, circle->circleHitbox[circleID].offsetX, circle->offsetZz, circle->offsetZx)) -
		(pos2x + RotateOffset(scaling2X * convex->convexHitbox[convexID].centerZ, scaling2Z * convex->convexHitbox[convexID].centerX, convex->offsetZz, convex->offsetZx));
	if (iShit2)
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
	if (iShit1)
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
		convex->onConvexCollision[convexID].CollisionFunction(params);
	}

	return true;

}

bool IsConvexCircularCollision(EntityID& entity1, EntityID& entity2, int convex, int circle)
{
	return IsCircularConvexCollision(entity2, entity1, circle, convex);
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

bool IsConvexCollision(EntityID& entity1, EntityID& entity2, int convexID1, int convexID2)
{
	TransformComponent* transform1 = registry.GetComponent<TransformComponent>(entity1);
	TransformComponent* transform2 = registry.GetComponent<TransformComponent>(entity2);
	float pos1x = 0.0f;
	float pos1z = 0.0f;
	float pos2x = 0.0f;
	float pos2z = 0.0f;
	float scaling1X = 1.0f;
	float scaling1Z = 1.0f;
	float scaling1 = 1.0f;
	float scaling2X = 1.0f;
	float scaling2Z = 1.0f;
	float scaling2 = 1.0f;
 	if (transform1 != nullptr)
	{
		pos1x = transform1->positionX;
		pos1z = transform1->positionZ;
		scaling1X = transform1->scaleX;
		scaling1 = scaling1X;
		scaling1Z = transform1->scaleZ;
		if (scaling1Z > scaling1)
		{
			scaling1 = scaling1Z;
		}
	}
	if (transform2 != nullptr)
	{
		pos2x = transform2->positionX;
		pos2z = transform2->positionZ;
		scaling2X = transform2->scaleX;
		scaling2 = scaling2X;
		scaling2Z = transform2->scaleZ;
		if (scaling2Z > scaling2)
		{
			scaling2 = scaling2Z;
		}
	}
	HitboxComponent *convex1 = registry.GetComponent<HitboxComponent>(entity1);
	HitboxComponent *convex2 = registry.GetComponent<HitboxComponent>(entity2);
	float center1X = convex1->convexHitbox[convexID1].centerX * scaling1X;
	float center1Z = convex1->convexHitbox[convexID1].centerZ * scaling1Z;
	float center2X = convex2->convexHitbox[convexID2].centerX * scaling2X;
	float center2Z = convex2->convexHitbox[convexID2].centerZ * scaling2Z;
	// get a hold of both the circles' bit flags to check if they can collide or not
	if (!convex1 || !convex2)
	{
		return false;
	} // add more check to more flags later, try with this first to see if the math works

	//Bitmask circle1 is to circle2 hit
	unsigned short iSmask = 0b0000000001111110;// is mask
	unsigned short hitMask = 0b0001111110000000;
	unsigned short* r1 = (unsigned short*)&convex1->convexFlags[convexID1];
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
	float posX = pos1x + RotateOffset(center1X, center1Z, convex1->offsetXx, convex1->offsetXz);
	float posZ = pos1z + RotateOffset(center1Z, center1X, convex1->offsetZz, convex1->offsetZx);
	float pos2X = pos2x + RotateOffset(center2X, center2Z, convex2->offsetXx, convex2->offsetXz);
	float pos2Z = pos2z + RotateOffset(center2Z, center2X, convex2->offsetZz, convex2->offsetZx);

	float dx = posX - pos2X;
	float dz = posZ - pos2Z;

	//If distance from each other is greater than radi, no circular collision, skip.
	if (sqrtf(dx * dx + dz * dz) > (scaling1 * convex1->convexHitbox[convexID1].boundingRadius) + (scaling2 * convex2->convexHitbox[convexID2].boundingRadius))
	{
		return false;
	}
	ConvexReturnData returnedData = {0};
	int whichLine = 0;
	//Now we can check which lines collide
	for (int i = 0; i < convex1->convexHitbox[convexID1].cornerAmount; i++)
	{
		for (int j = 0; j < convex2->convexHitbox[convexID2].cornerAmount; j++)
		{
			//Check from center to corner of one convex shape onto the sides of another.
			returnedData = LineToLineIntersection(pos1x + RotateOffset(center1X, center1Z, convex1->offsetXx, convex1->offsetXz),
				pos1z + RotateOffset(center1Z, center1X, convex1->offsetZz, convex1->offsetZx),
				pos1x + RotateOffset(convex1->convexHitbox[convexID1].cornerX[i] * scaling1X, convex1->convexHitbox[convexID1].cornerZ[i] * scaling1Z, convex1->offsetXx, convex1->offsetXz),
				pos1z + RotateOffset(convex1->convexHitbox[convexID1].cornerZ[i] * scaling1Z, convex1->convexHitbox[convexID1].cornerX[i] * scaling1X, convex1->offsetZz, convex1->offsetZx),
				pos2x + RotateOffset(convex2->convexHitbox[convexID2].cornerX[j] * scaling2X, convex2->convexHitbox[convexID2].cornerZ[j] * scaling2Z, convex2->offsetXx, convex2->offsetXz),
				pos2z + RotateOffset(convex2->convexHitbox[convexID2].cornerZ[j] * scaling2X, convex2->convexHitbox[convexID2].cornerX[j] * scaling2X, convex2->offsetZz, convex2->offsetZx),
				pos2x + RotateOffset(convex2->convexHitbox[convexID2].cornerX[(1 + j) % CONVEX_CORNER_LIMIT] * scaling2X, convex2->convexHitbox[convexID2].cornerZ[(1 + j) % CONVEX_CORNER_LIMIT] * scaling2Z,convex2->offsetXx, convex2->offsetXz),
				pos2z + RotateOffset(convex2->convexHitbox[convexID2].cornerZ[(1 + j) % CONVEX_CORNER_LIMIT] * scaling2Z, convex2->convexHitbox[convexID2].cornerX[(1 + j) % CONVEX_CORNER_LIMIT] * scaling2X, convex2->offsetZz, convex2->offsetZx));
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

	if (iShit2)
	{
		params.entity1 = entity1;
		params.entity2 = entity2;
		params.hitboxID1 = convexID1;
		params.hitboxID2 = convexID2;
		params.normal1X = -convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal1Z = -convex2->convexHitbox[convexID2].normalZ[whichLine];
		params.normal2X = convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal2Z = convex2->convexHitbox[convexID2].normalZ[whichLine];
		convex1->onConvexCollision[convexID1].CollisionFunction(params);
	}
	if (iShit1)
	{
		params.entity1 = entity2;
		params.entity2 = entity1;
		params.hitboxID1 = convexID1;
		params.hitboxID2 = convexID2;
		params.normal1X = convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal1Z = convex2->convexHitbox[convexID2].normalZ[whichLine];
		params.normal2X = -convex2->convexHitbox[convexID2].normalX[whichLine];
		params.normal2Z = -convex2->convexHitbox[convexID2].normalZ[whichLine];
		convex2->onConvexCollision[convexID2].CollisionFunction(params);
	}

	return returnedData.hit;
}

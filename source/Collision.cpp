#include "Backend/Collision.h"
#include "Backend/ConvexCollision.h"
#include "Backend/CircularCollision.h"
#include "Backend/ProximityCollision.h"
#include "EntityFramework.h"
#include "Registry.h"
#include <cmath>
#include "Systems\Systems.h"
#include "Components.h"
#include <fstream>
#include <sstream>
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Model.h"

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

VS_IDX hitboxVertexShader;
PS_IDX hitboxPixelShader;
RS_IDX hitboxRasterizerState;
CB_IDX hitboxConstantBuffer;

bool hitboxVisualizerActive[SAME_TYPE_HITBOX_LIMIT * 2];

struct CircularConvexReturn
{
	bool hit;
	int line;//0 means corners 0, 1. 1 means 1, 2. 2 means 2, 3 ir 2, 0 depending on amount of corners
	float convexAngleOfAttackX, convexAngleOfAttackY;
};

void SetupHitboxVisualizer()
{
	hitboxVertexShader = LoadVertexShader("DebugHitboxVertexShader.cso", LAYOUT_DESC::DEFAULT);
	hitboxPixelShader = LoadPixelShader("DebugHitboxPixelShader.cso");
	hitboxRasterizerState = CreateRasterizerState(false, false);
	hitboxConstantBuffer = CreateConstantBuffer(sizeof(SimpleShape));
	for (size_t i = 0; i < SAME_TYPE_HITBOX_LIMIT * 2; i++)
	{
		hitboxVisualizerActive[i] = false;
	}
#ifdef _DEBUG
	for (size_t i = 0; i < SAME_TYPE_HITBOX_LIMIT * 2; i++)
	{
		hitboxVisualizerActive[i] = true;
	}
#endif // _DEBUG


	// <>: Convex hitbox :)
	// []: Also convex hitbox
	// o: Circular hitboyx 
	// .: Smol circular hitbox :    ^ )
}

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
	for (auto entity : View<HitboxComponent, TransformComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ColliderComponent
	{
		HitboxComponent* hitboxes = registry.GetComponent<HitboxComponent>(entity);
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		if (hitboxes == nullptr || transform == nullptr)
		{
			continue;
		}
		float radians = acosf(transform->facingX);
		if (transform->facingZ < 0.0f)
		{
			radians *= -1.0f;
		}
		hitboxes->offsetXx = -sinf(radians);
		hitboxes->offsetZz = -sinf(radians);
		hitboxes->offsetXz = -cosf(radians);
		hitboxes->offsetZx = cosf(radians);
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
			//If none of the hitboxes can deal damage, check the next ones.
			if (!firstHitbox->circularFlags[i].canDealDamage && !firstHitbox->convexFlags[i].canDealDamage)
			{
				continue;
			}
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
					//If the circular hitbox can take damage, check collision with active hitboses
					if (secondHitbox->circularFlags[j].canTakeDamage)
					{
						
						if (firstHitbox->circularFlags[i].active && secondHitbox->circularFlags[j].active)
						{
							//Both are circular, do circle to circle.
							bool hit = IsCircularCollision(entity, entity2, i, j);
						}
						if (firstHitbox->convexFlags[i].active && secondHitbox->circularFlags[j].active)
						{
							//One is convex, other one is circular.
							bool hit = IsConvexCircularCollision(entity, entity2, i, j); //Could add a check for which is convex/circular if the parameter order matters.
						}
					}
					//If the convex hitbox can take damage, check collision with active hitboses
					if (secondHitbox->convexFlags[j].canTakeDamage)
					{ 
						if (firstHitbox->convexFlags[i].active && secondHitbox->convexFlags[j].active)
						{
							//Both are convex, do convex to convex.
							bool hit = IsConvexCollision(entity, entity2, i, j);
						}
						if (firstHitbox->circularFlags[i].active && secondHitbox->convexFlags[j].active)
						{
							//One is convex, other one is circular.
							bool hit = IsCircularConvexCollision(entity, entity2, i, j);//Could add a check for which is convex/circular if the parameter order matters.
						}
					}
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
					if (firstHitbox->circularFlags[i].isMoveable && secondHitbox->circularFlags[j].isMoveable && firstHitbox->circularFlags[i].active && secondHitbox->circularFlags[j].active)
					{
						//Both are circular, do circle to circle.
						bool hit = IsCircularCollision(entity, entity2, i, j);
					}
					if (firstHitbox->convexFlags[i].isMoveable && secondHitbox->convexFlags[j].isMoveable && firstHitbox->convexFlags[i].active && secondHitbox->convexFlags[j].active)
					{
						//Both are convex, do convex to convex.
						bool hit = IsConvexCollision(entity, entity2, i, j);
					}
					if (firstHitbox->circularFlags[i].isMoveable && secondHitbox->convexFlags[j].isMoveable && firstHitbox->circularFlags[i].active && secondHitbox->convexFlags[j].active)
					{
						//One is convex, other one is circular.
						bool hit = IsCircularConvexCollision(entity, entity2, i, j); //Could add a check for which is convex/circular if the parameter order matters.
					}
					if (firstHitbox->convexFlags[i].isMoveable && secondHitbox->circularFlags[j].isMoveable && firstHitbox->convexFlags[i].active && secondHitbox->circularFlags[j].active)
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
		int index = 0;
		EntityID closestWall = { 0 };
		closestWall.index = -1;
		HitboxComponent* entityHitbox = registry.GetComponent<HitboxComponent>(entity);
		TransformComponent* transformComponent = registry.GetComponent<TransformComponent>(entity);

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
					float currentDistance = sqrt(float((pow(transformComponent->positionX - wallHitbox->pointList[i].x, 2)) + (pow(transformComponent->positionZ - wallHitbox->pointList[i].z, 2))));
					
					if (distance > currentDistance) //Check if closer to the new point checked
					{
						distance = currentDistance;
						index = i;
						closestWall = entity2;
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
					float currentDistance = sqrt(float((pow(transformComponent->positionX - wallHitbox->pointList[i].x, 2)) + (pow(transformComponent->positionZ - wallHitbox->pointList[i].z, 2))));
					
					if (distance > currentDistance) //Check if closer to the new point checked
					{
						distance = currentDistance;
						index = i;
						closestWall = entity2;
					}
				}
			}
		}

		if (closestWall.index != -1) //If an entity has been assigned as the closestWall
		{
			ProximityCorrection(closestWall, index, transformComponent->positionX, transformComponent->positionZ, transformComponent->lastPositionX, transformComponent->lastPositionZ);
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

void ProximityHitboxComponent::Load(const char* fileName)
{
	//Read hitbox file if it exists
	std::string hitboxFileName = fileName;
	hitboxFileName = "HitboxFiles/" + hitboxFileName + ".box";
	std::ifstream file(hitboxFileName);
	if (file.is_open())
	{
		//Read file contents and add to CHV
		std::string line = "";
		std::string value = "";
		ProximityPoint currentPoint;
		currentPoint.index = 0;
		currentPoint.x = 0;
		currentPoint.z = 0;

		while (std::getline(file, line))
		{
			std::stringstream ss(line);

			//Get prefix
			std::string prefix;
			ss >> prefix;

			if (prefix == "Point")
			{
				ss >> value;
				currentPoint.index = (int)std::stoi(value);
			}
			if (prefix == "X")
			{
				ss >> value;
				currentPoint.x = (float)std::stoi(value);
			}
			else if (prefix == "Z")
			{
				ss >> value;
				currentPoint.z = (float)std::stoi(value);
				this->pointList.push_back(currentPoint);
			}
			else if (prefix == "Clockwise")
			{
				ss >> value;
				this->clockwise = std::stoi(value);
			}
		}
		file.close();
	}
}

void UpdatePhysics()
{
	ResetCollisionVariables();
	HandleMoveableCollision();
	HandleDamageCollision();
	HandleStaticCollision();
}

float RotateOffset(float offsetX, float offsetZ, float xFactor, float zFactor)
{
	return offsetX * xFactor + offsetZ * zFactor;
}

int8_t GetHitboxVisVertexShader()
{
	return hitboxVertexShader;
}

int8_t GetHitboxVisPixelShader()
{
	return hitboxPixelShader;
}

int8_t GetHitboxRasterizerState()
{
	return hitboxRasterizerState;
}
int16_t GetHitboxConstantBuffer()
{
	return hitboxConstantBuffer;
}

bool CollisionSystem::Update()
{
	UpdatePhysics();
	return true;
}


float GetHitboxRadius(const EntityID& entity, int hitBoxID)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitBoxID < SAME_TYPE_HITBOX_LIMIT)
	{
		return hitbox->circleHitbox[hitBoxID].radius;
	}
	else
	{
		return hitbox->convexHitbox[hitBoxID - SAME_TYPE_HITBOX_LIMIT].boundingRadius;
	}
	return 0;
}

void VisualizeHitbox(EntityID& entity, int hitboxID)
{
	//Visualize only this hitbox of this entity.
	HitboxVisualComponent* vis = registry.GetComponent<HitboxVisualComponent>(entity);
	if (vis == nullptr)
	{
		vis = registry.AddComponent<HitboxVisualComponent>(entity);
	}
	float red, green, blue;
	switch (hitboxID)
	{
	case 0:
		red = 0.2f;
		green = 0.8f;
		blue = 0.3f;
		break;
	case 1:
		red = 0.25f;
		green = 0.4f;
		blue = 1.0f;
		break;
	case 2:
		red = 1.0f;
		green = 0.3f;
		blue = 0.3f;
		break;
	case 4:
		red = 1.0f;
		green = 1.0f;
		blue = 0.2f;
		break;
	default:
		red = 1.0f;
		green = 1.0f;
		blue = 1.0f;
		break;
	}
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	float height = 0.0f;
	//Loop through the correct hitbox values
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		vis->shape[hitboxID].nrVertices = CONVEX_CORNER_LIMIT + 1;
		
		for (size_t i = 0; i < CONVEX_CORNER_LIMIT; i++)
		{
			vis->shape[hitboxID].vertices[i].color[0] = red;
			vis->shape[hitboxID].vertices[i].color[1] = green;
			vis->shape[hitboxID].vertices[i].color[2] = blue;
			vis->shape[hitboxID].vertices[i].color[3] = 1.0f;
			//DO the gf
			vis->shape[hitboxID].vertices[i].position[0] = hitbox->circleHitbox[hitboxID].offsetX + hitbox->circleHitbox[hitboxID].radius * cosf(3.1415f * 2.0f *((float)i / (float)CONVEX_CORNER_LIMIT));
			vis->shape[hitboxID].vertices[i].position[1] = height;
			vis->shape[hitboxID].vertices[i].position[2] = hitbox->circleHitbox[hitboxID].offsetZ + hitbox->circleHitbox[hitboxID].radius * sinf(3.1415f * 2.0f *((float)i / (float)CONVEX_CORNER_LIMIT));
			vis->shape[hitboxID].vertices[i].position[3] = 1.0f;
		}
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].color[0] = red;
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].color[1] = green;
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].color[2] = blue;
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].color[3] = 1.0f;
		//DO the gf					  CONVEX_CORNER_LIMIT
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].position[0] = hitbox->circleHitbox[hitboxID].offsetX + hitbox->circleHitbox[hitboxID].radius;
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].position[1] = height;
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].position[2] = hitbox->circleHitbox[hitboxID].offsetZ;
		vis->shape[hitboxID].vertices[CONVEX_CORNER_LIMIT].position[3] = 1.0f;
	}
	else
	{
		vis->shape[hitboxID].nrVertices = hitbox->convexHitbox[hitboxID - SAME_TYPE_HITBOX_LIMIT].cornerAmount;
		for (size_t i = 0; i < vis->shape[hitboxID].nrVertices; i++)
		{
			vis->shape[hitboxID].vertices[i].color[1] = 1.0f;
			vis->shape[hitboxID].vertices[i].color[0] = 0.2f;
			vis->shape[hitboxID].vertices[i].color[2] = 0.2f;
			vis->shape[hitboxID].vertices[i].color[3] = 1.0f;
			//thisO the[hitboxID] gf
			vis->shape[hitboxID].vertices[i].position[0] = hitbox->convexHitbox[hitboxID - SAME_TYPE_HITBOX_LIMIT].cornerX[i];
			vis->shape[hitboxID].vertices[i].position[1] = height;
			vis->shape[hitboxID].vertices[i].position[2] = hitbox->convexHitbox[hitboxID - SAME_TYPE_HITBOX_LIMIT].cornerZ[i];
			vis->shape[hitboxID].vertices[i].position[3] = 1.0f;
		}
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].color[0] = 1.0f;
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].color[1] = 0.2f;
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].color[2] = 0.2f;
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].color[3] = 1.0f;
		//vis->//the gf				vis->	  CONVEX_CORNER_LIMIT
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].position[0] = vis->shape[hitboxID].vertices[0].position[0];
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].position[1] = vis->shape[hitboxID].vertices[0].position[1];
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].position[2] = vis->shape[hitboxID].vertices[0].position[2];
		vis->shape[hitboxID].vertices[vis->shape[hitboxID].nrVertices].position[3] = vis->shape[hitboxID].vertices[0].position[3];
		++vis->shape[hitboxID].nrVertices;
	}
}

void StopVisualizeHitbox(EntityID& entity)
{
	HitboxVisualComponent* vis = registry.GetComponent<HitboxVisualComponent>(entity);
	if (vis != nullptr)
	{
		HitboxVisualComponent empty = {0};
		memcpy(vis, &empty, sizeof(HitboxVisualComponent));
		registry.RemoveComponent<HitboxVisualComponent>(entity);//No need to release anything. Deleetus Yeetus
	}
}

int HitboxVisualComponent::GetNrVertices(EntityID& entity, int hitboxID)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitboxID < SAME_TYPE_HITBOX_LIMIT)
	{
		if (hitbox->circularFlags[hitboxID].active)
		{
			return this->shape[hitboxID].nrVertices;
		}
	}
	else
	{
		if (hitbox->convexFlags[hitboxID - SAME_TYPE_HITBOX_LIMIT].active)
		{
			return this->shape[hitboxID].nrVertices;
		}
	}
	return 0;
}

void HitboxVisualComponent::UpdateHitboxConstantBuffer(EntityID& entity, int hitboxID)
{
	HitboxVisualComponent* vis = registry.GetComponent<HitboxVisualComponent>(entity);
	if (vis && vis->GetNrVertices(entity, hitboxID) > 0)
	{
		VisualizeHitbox(entity, hitboxID);
		UpdateConstantBuffer(hitboxConstantBuffer, &this->shape[hitboxID].vertices);
	}
	
}


//EntityID CreateStaticHazard(const StaticHazardType& type, const char* model,
//	const float& positionX, const float& positionY, const float& positionZ,
//	const float& scaleX, const float& scaleY, const float& scaleZ,
//	const float& colorAddR,const float& colorAddG,const float& colorAddB,
//	const float& colorMulR, const float& colorMulG, const float& colorMulB, 
//	const float& gammaCorrection, const float& facingX, const float& facingZ)
//{
//	EntityID hazard = registry.CreateEntity();
//	ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel(model));
//	hazardModel->colorAdditiveRed = colorAddR;
//	hazardModel->colorAdditiveGreen = colorAddG;
//	hazardModel->colorAdditiveBlue = colorAddB;
//	hazardModel->colorMultiplicativeRed = colorMulR;
//	hazardModel->colorMultiplicativeGreen = colorMulG;
//	hazardModel->colorMultiplicativeBlue = colorMulB;
//	hazardModel->gammaCorrection = gammaCorrection;
//	hazardModel->castShadow = false;
//	TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
//	hazardTransform->positionX = positionX;
//	hazardTransform->positionY = positionY;
//	hazardTransform->positionZ = positionZ;
//	hazardTransform->scaleX = scaleX;
//	hazardTransform->scaleY = scaleY;
//	hazardTransform->scaleZ = scaleZ;
//	hazardTransform->facingX = facingX;
//	hazardTransform->facingZ = facingZ;
//	AddStaticHazard(hazard, type);
//	return hazard;
//}

void SetHitboxRadius(const EntityID& entity, int hitBoxID, const float r)
{
	HitboxComponent* hitbox = registry.GetComponent<HitboxComponent>(entity);
	if (hitBoxID < SAME_TYPE_HITBOX_LIMIT)
	{
		hitbox->circleHitbox[hitBoxID].radius = r;
	}
	else
	{
		hitbox->convexHitbox[hitBoxID - SAME_TYPE_HITBOX_LIMIT].boundingRadius = r;
	}
}

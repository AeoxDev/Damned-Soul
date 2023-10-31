#include "EntityFramework.h"
#include "Components.h"
#include "Registry.h"
#include "EventFunctions.h"
#include "EntityCreation.h"
#include "CollisionFunctions.h"
#include "Model.h"
EntityID CreateRoundStaticHazard(const char* file, const float& posX, const float& posY, const float& posZ, const float& scaleX, const float& scaleY, const float& scaleZ, const float& colorRed, const float& colorGreen, const float& colorBlue, const float& radius, const float& damage, const float& rotationY)
{
	EntityID hazard = registry.CreateEntity();
	ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("PlaceholderScene.mdl"));
	hazardModel->colorAdditiveRed = colorRed;
	hazardModel->colorAdditiveGreen = colorGreen;
	hazardModel->colorAdditiveBlue = colorBlue;
	TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
	hazardTransform->positionX = posX;
	hazardTransform->positionY = posY;
	hazardTransform->positionZ = posZ;
	hazardTransform->scaleX = scaleX;
	hazardTransform->scaleY = scaleY;
	hazardTransform->scaleZ = scaleZ;
	hazardTransform->facingX = cosf(rotationY);
	hazardTransform->facingZ = sinf(rotationY);

	SetupLavaCollisionBox(hazard, radius);
	registry.AddComponent<StatComponent>(hazard, 0.1f, 0.0f, damage, 1.0f);//Hp, MoveSpeed, Damage, AttackSpeed
	AddTimedEventComponentStart(hazard, 0.1f, LoopHitbox, 2);
	CreatePointLight(hazard, colorRed, colorGreen, colorBlue, 0.0f, 0.1f, 0.0f, radius, 1.0f);
	return hazard;
}

EntityID CreateSquareStaticHazard(const char* file, const float& posX, const float& posY, const float& posZ, const float& scaleX, const float& scaleY, const float& scaleZ, const float& corner1X, const float& corner1Y, const float& corner2X, const float& corner2Y, const float& corner3X, const float& corner3Y, const float& corner4X, const float& corner4Y, const float& damage, const float& rotationY,
	const float& colorAdditiveRed, const float& colorAdditiveGreen, const float& colorAdditiveBlue, const float& colorMultiRed, const float& colorMultiGreen, const float& colorMultiBlue)
{
	EntityID hazard = registry.CreateEntity();
	ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel(file));
	hazardModel->colorAdditiveRed = colorAdditiveRed;
	hazardModel->colorAdditiveGreen = colorAdditiveGreen;
	hazardModel->colorAdditiveBlue = colorAdditiveBlue;
	hazardModel->colorMultiplicativeRed = colorMultiRed;
	hazardModel->colorMultiplicativeGreen = colorMultiGreen;
	hazardModel->colorMultiplicativeBlue = colorMultiBlue;

	TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
	hazardTransform->positionX = posX;
	hazardTransform->positionY = posY;
	hazardTransform->positionZ = posZ;
	hazardTransform->scaleX = scaleX;
	hazardTransform->scaleY = scaleY;
	hazardTransform->scaleZ = scaleZ;
	hazardTransform->facingX = cosf(rotationY);
	hazardTransform->facingZ = sinf(rotationY);
	//Create convex shape
	AddHitboxComponent(hazard);
	float cornersX[4] = { corner1X, corner2X, corner3X, corner4X };
	float cornersY[4] = { corner1Y, corner2Y, corner3Y, corner4Y };
	int staticID = CreateHitbox(hazard, 4, cornersX, cornersY);
	if (staticID < 0)
	{
		return EntityID{-1, false};
	}
	SetCollisionEvent(hazard, staticID, StaticHazardAttackCollision);
	SetHitboxHitEnemy(hazard, staticID);
	SetHitboxHitPlayer(hazard, staticID);
	//SetHitboxIsStaticHazard(hazard, staticID);//Only relevant if this is should have clipping collision.
	SetHitboxActive(hazard, staticID);
	//SetHitboxCanTakeDamage(hazard, staticID, false);
	SetHitboxCanDealDamage(hazard, staticID, true);
	//SetHitboxIsMoveable(hazard, staticID, true);

	registry.AddComponent<StatComponent>(hazard, 0.1f, 0.0f, damage, 1.0f);//Hp, MoveSpeed, Damage, AttackSpeed
	AddTimedEventComponentStart(hazard, 0.2f, LoopHitbox, 2);
	//CreatePointLight(hazard, colorRed, colorGreen, colorBlue, 0.0f, 0.1f, 0.0f, scaleX, 1.0f);
	return hazard;
}

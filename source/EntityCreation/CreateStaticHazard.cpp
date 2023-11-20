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
	hazardModel->shared.colorAdditiveRed = colorRed;
	hazardModel->shared.colorAdditiveGreen = colorGreen;
	hazardModel->shared.colorAdditiveBlue = colorBlue;
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


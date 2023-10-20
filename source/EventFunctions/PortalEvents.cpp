#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Level.h"
#include "CollisionFunctions.h" //NextLevel
#include "States\StateManager.h"

void CreatePortal(EntityID& entity, const int& index)
{
	//Create a model and a transform for portal:
	registry.AddComponent<PointOfInterestComponent>(entity);
	CreatePointLight(entity, 0.0f, 6.0f, 5.0f, 0.0f, 2.0f, -1.0f, 64.0f, 10.0f); 
	TransformComponent* portalTransform = registry.AddComponent<TransformComponent>(entity);
	portalTransform->positionX = -77.0f;
	portalTransform->positionZ = -34.0f;
	portalTransform->scaleY = 10.0f;
	portalTransform->scaleX = 0.1f;
	portalTransform->scaleZ = 2.0f;
	AddHitboxComponent(entity);
	float cornersX[] = {-0.2f, 0.2f, 0.2f, -0.2f};
	float cornersZ[] = { -4.0f, -4.0f, 4.0f, 4.0f};
	int hitbox = CreateHitbox(entity, 4, cornersX, cornersZ);
	SetHitboxHitPlayer(entity, hitbox, true);
	SetHitboxActive(entity, hitbox, true);
	SetHitboxIsMoveable(entity, hitbox, true);
	SetCollisionEvent(entity, hitbox, LoadNextLevel);
	//ModelBonelessComponent* portalModel = registry.AddComponent<ModelBonelessComponent>(entity);
	//portalModel->model = LoadModel("Boss.mdl");
}

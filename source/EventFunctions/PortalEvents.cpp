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
	TransformComponent* portalTransform = registry.AddComponent<TransformComponent>(entity);
	portalTransform->positionX = -78.0f;
	portalTransform->positionZ = -32.0f;
	portalTransform->scaleY = 10.0f;
	portalTransform->facingZ = 0.0f;
	portalTransform->facingX = -1.0f;
	AddHitboxComponent(entity);
	float cornersX[] = {-0.2f, 0.2f, 0.2f, -0.2f};
	float cornersZ[] = { -4.0f, -4.0f, 2.0f, 2.0f};
	int hitbox = CreateHitbox(entity, 4, cornersX, cornersZ);
	SetHitboxHitPlayer(entity, hitbox, true);
	SetHitboxActive(entity, hitbox, true);
	SetHitboxIsMoveable(entity, hitbox, true);
	SetCollisionEvent(entity, hitbox, LoadNextLevel);
	ModelBonelessComponent* portalModel = registry.AddComponent<ModelBonelessComponent>(entity);
	portalModel->model = LoadModel("Boss.mdl");
}

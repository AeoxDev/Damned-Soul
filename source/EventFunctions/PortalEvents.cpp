#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Level.h"
#include "CollisionFunctions.h" //NextLevel
#include "States\StateManager.h"

void CreatePortal(EntityID& entity)
{
	//Create a model and a transform for portal:
	TransformComponent* portalTransform = registry.AddComponent<TransformComponent>(entity);
	ModelBonelessComponent* portalModel = registry.AddComponent<ModelBonelessComponent>(entity);
	AddHitboxComponent(entity);
	float cornersX[] = {-0.2f, 0.2f, 0.2f, -0.2f};
	float cornersZ[] = { -2.0f, -2.0f, 2.0f, 2.0f};
	int hitbox = CreateHitbox(entity, 4, cornersX, cornersZ);
	SetHitboxHitPlayer(entity, hitbox, true);
	SetHitboxActive(entity, hitbox, true);
	SetHitboxIsStage(entity, hitbox);
	SetHitboxIsMoveable(entity, hitbox, true);
	SetCollisionEvent(entity, hitbox, LoadNextLevel);
	portalModel->model = LoadModel("Boss.mdl");
	portalTransform->positionX = -78.0f;
	portalTransform->positionZ = -32.0f;
	portalTransform->scaleY = 10.0f;
}

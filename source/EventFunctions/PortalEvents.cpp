#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Level.h"
#include "CollisionFunctions.h" //NextLevel
#include "States\StateManager.h"
#include "Model.h"

void CreatePortal(EntityID& entity, const int& index)
{
	//Create a model and a transform for portal:
	registry.AddComponent<PointOfInterestComponent>(entity);
	TransformComponent* portalTransform = registry.AddComponent<TransformComponent>(entity);
	//CreatePointLight(entity, 0.0f, 6.0f, 5.0f, 0.0f, 2.0f, -1.0f, 64.0f, 10.0f); 
	CreatePointLight(entity, 0.0f, 3.0f, 2.0f, 0.0f, 2.0f, 0.0f, 180.0f, 20.0f); 
	if (stateManager.gate.index != -1)
	{
		registry.DestroyEntity(stateManager.gate);
		stateManager.gate.index = -1;
	}
	
	switch (stateManager.activeLevel)
	{
	case 1://Level1
		portalTransform->positionX = -250.0f;
		portalTransform->positionZ = 81.0f;
		portalTransform->positionY = 11.0f;
		portalTransform->facingZ = 0.5f;
		portalTransform->facingX = 0.5f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 3.0f;
		break;
	case 3://Level2
 		portalTransform->positionX = -277.0f;
		portalTransform->positionZ = 258.0f;
		portalTransform->positionY = 5.0f;
		portalTransform->facingZ = 0.5f;
		portalTransform->facingX = 0.5f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 3.0f;
		break;
	case 5://Level3
		portalTransform->positionX = -270.0f;
		portalTransform->positionZ = 143.0f;
		portalTransform->positionY = 9.0f;
		portalTransform->facingZ = 0.6f;
		portalTransform->facingX = 0.4f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 3.0f;
		break;
	case 7://Level4
		portalTransform->positionX = -144.0f;
		portalTransform->positionZ = 107.0f;
		portalTransform->positionY = 11.0f;
		portalTransform->facingZ = 0.5f;
		portalTransform->facingX = 0.5f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 4.0f;
		break;
	case 9://Level5
		portalTransform->positionX = -378.0f;
		portalTransform->positionZ = 217.0f;
		portalTransform->positionY = 9.0f;
		portalTransform->facingZ = 0.5f;
		portalTransform->facingX = 0.5f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 4.0f;
		break;
	case 11://Level6
		portalTransform->positionX = -434.0f;
		portalTransform->positionZ = 452.0f;
		portalTransform->positionY = 10.0f;
		portalTransform->facingZ = 0.5f;
		portalTransform->facingX = 0.5f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 4.0f;
		break;
	case 13://Level7
		portalTransform->positionX = -235.0f;
		portalTransform->positionZ = 268.0f;
		portalTransform->positionY = 15.0f;
		portalTransform->facingZ = 0.5f;
		portalTransform->facingX = 0.5f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 4.0f;
		break;
	case 15://Level8
		portalTransform->positionX = -0.0f;
		portalTransform->positionZ = 134.0f;
		portalTransform->positionY = 11.0f;
		portalTransform->facingZ = -1.0f;
		portalTransform->facingX = 0.0f;
		portalTransform->scaleY = 1.0f;
		portalTransform->scaleX = 1.0f;
		portalTransform->scaleZ = 4.0f;
		break;
	default:
		portalTransform->positionX = -77.0f;
		portalTransform->positionZ = -34.0f;
		portalTransform->scaleY = 10.0f;
		portalTransform->scaleX = 0.1f;
		portalTransform->scaleZ = 2.0f;
		break;
	}
	


	NormalizeFacing(portalTransform);
	//AddHitboxComponent(entity);
	//float cornersX[] = {-0.2f, 0.2f, 0.2f, -0.2f};//-242, 90; -259, 72
	//float cornersZ[] = { -4.0f, -4.0f, 4.0f, 4.0f};
	//int hitbox = CreateHitbox(entity, 4, cornersX, cornersZ);
	//SetHitboxHitPlayer(entity, hitbox, true);
	//SetHitboxActive(entity, hitbox, true);
	//SetHitboxIsMoveable(entity, hitbox, true);
	//SetCollisionEvent(entity, hitbox, LoadNextLevel);
	//ModelBonelessComponent* portalModel = registry.AddComponent<ModelBonelessComponent>(entity);
	//portalModel->model = LoadModel("Boss.mdl");
}

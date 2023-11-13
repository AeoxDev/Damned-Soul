#include "CutsceneComponent.h"
#include "Registry.h"
#include "Camera.h"
#include "Components.h"
#include "EventFunctions.h"
void MainMenuIntroCutscene(EntityID& entity, const int& index)
{
	float pointX = (rand() % 64) * 0.5f - 32.0f;
	float pointZ = (rand() % 64) * 0.5f - 32.0f;
	for (auto enemy : View<EnemyComponent, TransformComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(enemy);
		pointX = transform->positionX;
		pointZ = transform->positionZ;
		if (rand()%2)
		{
			break;
		}

	}
	//Zoom from 0 to to camera offset
	CutsceneComponent* cutscene = registry.AddComponent<CutsceneComponent>(entity);
	
	float posRandX = (rand() % 128) * 0.5f - 64.0f;
	float posRandY = (rand() % 256) * 0.2f;
	float posRandZ = (rand() % 128) * 0.5f - 64.0f;
	
	


	cutscene->startLookAtX = pointX;
	cutscene->startLookAtY = 0.0f;
	cutscene->startLookAtZ = pointZ;
	cutscene->startPositionX = posRandX;
	cutscene->startPositionY = posRandY + 0.1f * CAMERA_OFFSET_Y;
	cutscene->startPositionZ = posRandZ;
	posRandX = (rand() % 128) * 0.5f - 64.0f;
	posRandZ = (rand() % 128) * 0.5f - 64.0f;
	
	cutscene->goalPositionX = posRandX;
	cutscene->goalPositionY = 1.2f + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = posRandZ;
	float randX = (rand() % 128) * 0.5f - 64.0f;
	float randY = 0.0f;
	float randZ = (rand() % 128) * 0.5f - 64.0f;
	for (auto enemy : View<EnemyComponent, TransformComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(enemy);
		randX = transform->positionX;
		randZ = transform->positionZ;
		if (rand() % 4)
		{
			break;
		}

	}
	cutscene->goalLookAtX = randX;
	cutscene->goalLookAtY = randY;
	cutscene->goalLookAtZ = randZ;
	cutscene->mode = (CutsceneMode)(CutsceneMode::Cutscene_Linear | CutsceneMode::Transition_Position | Transition_LookAt);
	float randTime = (float)(rand() % 32) + 8.0f;
	float randTime2 = (float)(rand() % 32) + 8.0f;
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, randTime, EndCutscene, 0, 1);
	AddTimedEventComponentStart(entity,randTime + (randTime2 * ((float)(rand()%2))), MainMenuIntroCutscene, 0, 2);
}

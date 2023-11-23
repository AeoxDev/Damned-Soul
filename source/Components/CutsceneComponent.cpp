#include "CutsceneComponent.h"
#include "Registry.h"
#include "Camera.h"
#include "Components.h"
#include "EventFunctions.h"
#include "DeltaTime.h"
#include "States\StateManager.h"
void MainMenuIntroCutscene(EntityID& entity, const int& index)
{
	float pointX = (rand() % 64) * 0.5f - 32.0f;
	float pointZ = (rand() % 64) * 0.5f - 32.0f;
	for (auto enemy : View<EnemyComponent, TransformComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(enemy);
		pointX = transform->positionX;
		pointZ = transform->positionZ;
		if (rand()%16)
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
	cutscene->startPositionY = posRandY + 0.33f * CAMERA_OFFSET_Y;
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
	float randTime = (float)(rand() % 16) + 8.0f;
	float randTime2 = (float)(rand() % 16) + 8.0f;
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, randTime, EndCutscene, 0, 2);
	AddTimedEventComponentStart(entity,randTime + (randTime2 * ((float)(rand()%2))), MainMenuIntroCutscene, 0, 2);
}

void StageIntroFall(EntityID& entity, const int& index)
{
	//Fall from a high place down to 0.
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;
	float returnTime = 1.0f;
	float fallTime = 0.85f;
	float pauseTime = fallTime + 0.3f;
	float fallHeight = 60.0f;
	//Fall of point. -321.6, 133.14
	//Move camera to view point and background from a lower angle
	EntityID cutscene1 = registry.CreateEntity();
	CutsceneComponent* sidewaysCut = registry.AddComponent<CutsceneComponent>(cutscene1);
	sidewaysCut->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Linear);
	CutsceneSetLookAt(cutscene1, 0.0f, fallHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	CutsceneSetPosition(cutscene1, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(cutscene1, 0.0f, BeginCutscene, CutsceneTransition, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Fall down
	CutsceneComponent* fall = registry.AddComponent<CutsceneComponent>(entity);
	fall->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Cutscene_Character_Fall | CutsceneMode::Cutscene_Linear);
	CutsceneSetPosition(entity, 0.0f, fallHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	

	EntityID returnCamera = registry.CreateEntity();
	CutsceneComponent* returnCutscene = registry.AddComponent<CutsceneComponent>(returnCamera);
	returnCutscene->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Linear);
	CutsceneSetPosition(returnCamera, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z, CAMERA_OFFSET_X, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z);
	CutsceneSetLookAt(returnCamera, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	
	AddTimedEventComponentStartContinuousEnd(returnCamera, fallTime, BeginCutscene, CutsceneTransition, pauseTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, pauseTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void Stage1IntroScene(EntityID& entity, const int& index)
{
	//Look at skeletons!
	//Then spawn player
	CutsceneNPCIntro1NoText(entity);
}


void CutsceneNPCIntro2NoText(EntityID& entity);
void HellhoundIntroScene(EntityID& entity, const int& index)
{
	//Look at dog!
	CutsceneNPCIntro2NoText(entity);
}

void CutsceneNPCIntro3NoText(EntityID& entity);
void ImpIntroScene(EntityID& entity, const int& index)
{
	CutsceneNPCIntro3NoText(entity);
}

void CutsceneNPCIntro4NoText(EntityID& entity);
void Stage4IntroScene(EntityID& entity, const int& index)
{
	if ((rand() % 16) == 0)
	{
		CutsceneNPCIntro4NoText(entity);
	}
	CutsceneNPCIntro1NoText(entity);
}



void CutsceneSetPosition(EntityID& entity, float startX, float startY, float startZ, float goalX, float goalY, float goalZ)
{
	CutsceneComponent* cutscene = registry.GetComponent<CutsceneComponent>(entity);
	if (cutscene == nullptr)
	{
		return;
	}
	cutscene->startPositionX = startX;
	cutscene->startPositionY = startY;
	cutscene->startPositionZ = startZ;
	cutscene->goalPositionX = goalX;
	cutscene->goalPositionY = goalY;
	cutscene->goalPositionZ = goalZ;
}

void CutsceneSetLookAt(EntityID& entity, float startX, float startY, float startZ, float goalX, float goalY, float goalZ)
{
	CutsceneComponent* cutscene = registry.GetComponent<CutsceneComponent>(entity);
	if (cutscene == nullptr)
	{
		return;
	}
	cutscene->startLookAtX = startX;
	cutscene->startLookAtY = startY;
	cutscene->startLookAtZ = startZ;
	cutscene->goalLookAtX = goalX;
	cutscene->goalLookAtY = goalY;
	cutscene->goalLookAtZ = goalZ;
}

//One overhead to legs, one side to side, one zoom in, then return
void CutsceneNPCIntro1NoText(EntityID& entity)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;
	
	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);
	cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 6.0f;
	cutscene->startLookAtZ = transform->positionZ;
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	float cameraXOffset = transform->facingX * 15.0f;
	float cameraYOffset = 0.0f;
	float cameraZOffset = transform->facingZ * 15.0f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + cameraXOffset;
	cutscene->startPositionY = transform->positionY + cameraYOffset + 6.0f;
	cutscene->startPositionZ = transform->positionZ + cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset;
	cutscene->goalPositionY = transform->positionY + cameraYOffset + 2.0f;
	cutscene->goalPositionZ = transform->positionZ + cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, 1.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN ,2);

	EntityID additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + 2.0f;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 2.0f;
	cutscene->startLookAtZ = transform->positionZ;

	cameraXOffset = transform->facingX * 25.0f;
	cameraYOffset = 4.5f;
	cameraZOffset = transform->facingZ * 25.0f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + cameraXOffset + cameraZOffset;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset - cameraXOffset;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ + cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 1.0f, BeginCutscene, CutsceneTransition, 2.5f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	
	//Zoom into the enemy
	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + 4.5f;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 4.5f;
	cutscene->startLookAtZ = transform->positionZ;
	cameraYOffset = 4.5f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + transform->facingX * 60.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 60.0f;
	cutscene->goalPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 2.5f, BeginCutscene, CutsceneTransition, 4.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transformPlayer->positionX;
	cutscene->goalLookAtY = transformPlayer->positionY;
	cutscene->goalLookAtZ = transformPlayer->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 4.5f;
	cutscene->startLookAtZ = transform->positionZ;

	//Do one cutscene, then the other
	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();
	cutscene->startPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	cutscene->goalPositionX = transformPlayer->positionX + CAMERA_OFFSET_X;
	cutscene->goalPositionY = transformPlayer->positionY + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = transformPlayer->positionZ + CAMERA_OFFSET_Z;
	AddTimedEventComponentStartContinuousEnd(returnTransition, 4.4f, BeginCutscene, CutsceneTransition, 5.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, 5.5f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

//Zoom in, look at back to front, then return
void CutsceneNPCIntro2NoText(EntityID& entity)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);
	cutscene = registry.AddComponent<CutsceneComponent>(entity);

	float cameraXOffset = transform->facingX * 15.0f;
	float cameraYOffset = 3.5;
	float cameraZOffset = transform->facingZ * 15.0f;

	//Zoom into the enemy
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + cameraYOffset;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + cameraYOffset;
	cutscene->startLookAtZ = transform->positionZ;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + transform->facingX * 60.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 60.0f;
	cutscene->goalPositionX = transform->positionX + transform->facingX * 5.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ + transform->facingZ * 5.0f;
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, 1.5f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	

	EntityID additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + 2.0f;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 2.0f;
	cutscene->startLookAtZ = transform->positionZ;

	cameraXOffset = transform->facingX * 25.0f;
	cameraYOffset = 4.5f;
	cameraZOffset = transform->facingZ * 25.0f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + cameraXOffset + cameraZOffset;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset - cameraXOffset;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ + cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 1.6f, BeginCutscene, CutsceneTransition, 3.5f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transformPlayer->positionX;
	cutscene->goalLookAtY = transformPlayer->positionY;
	cutscene->goalLookAtZ = transformPlayer->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 4.5f;
	cutscene->startLookAtZ = transform->positionZ;

	//Do one cutscene, then the other
	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();
	cutscene->startPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	cutscene->goalPositionX = transformPlayer->positionX + CAMERA_OFFSET_X;
	cutscene->goalPositionY = transformPlayer->positionY + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = transformPlayer->positionZ + CAMERA_OFFSET_Z;
	AddTimedEventComponentStartContinuousEnd(returnTransition, 3.6f, BeginCutscene, CutsceneTransition, 5.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, 5.1f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

//One overhead to legs, one side to side, one zoom in, then return
void CutsceneNPCIntro3NoText(EntityID& entity)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);
	cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + 6.0f;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY;
	cutscene->startLookAtZ = transform->positionZ;
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	float cameraXOffset = transform->facingX * 15.0f;
	float cameraYOffset = 0.0f;
	float cameraZOffset = transform->facingZ * 15.0f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + cameraXOffset;
	cutscene->startPositionY = transform->positionY + cameraYOffset + 2.0f;
	cutscene->startPositionZ = transform->positionZ + cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset;
	cutscene->goalPositionY = transform->positionY + cameraYOffset + 6.0f;
	cutscene->goalPositionZ = transform->positionZ + cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, 1.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	EntityID additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + 2.0f;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 2.0f;
	cutscene->startLookAtZ = transform->positionZ;

	cameraXOffset = transform->facingX * 25.0f;
	cameraYOffset = 2.5f;
	cameraZOffset = transform->facingZ * 25.0f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + cameraXOffset - cameraZOffset;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset + cameraXOffset;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ + cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 1.0f, BeginCutscene, CutsceneTransition, 2.5f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Zoom into the enemy
	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + 2.5f;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 2.5f;
	cutscene->startLookAtZ = transform->positionZ;
	cameraYOffset = 2.5f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + transform->facingX * 60.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 60.0f;
	cutscene->goalPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 2.5f, BeginCutscene, CutsceneTransition, 4.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transformPlayer->positionX;
	cutscene->goalLookAtY = transformPlayer->positionY;
	cutscene->goalLookAtZ = transformPlayer->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 2.5f;
	cutscene->startLookAtZ = transform->positionZ;

	//Do one cutscene, then the other
	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();
	cutscene->startPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	cutscene->goalPositionX = transformPlayer->positionX + CAMERA_OFFSET_X;
	cutscene->goalPositionY = transformPlayer->positionY + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = transformPlayer->positionZ + CAMERA_OFFSET_Z;
	AddTimedEventComponentStartContinuousEnd(returnTransition, 4.4f, BeginCutscene, CutsceneTransition, 5.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, 5.5f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

void CutsceneNPCIntro4NoText(EntityID& entity)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);
	cutscene = registry.AddComponent<CutsceneComponent>(entity);

	float cameraXOffset = transform->facingX * 15.0f;
	float cameraYOffset = 18.0f;
	float cameraZOffset = transform->facingZ * 15.0f;

	//Zoom into the enemy
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + cameraYOffset;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + cameraYOffset;
	cutscene->startLookAtZ = transform->positionZ;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX - transform->facingX * 256.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ - transform->facingZ * 256.0f;
	cutscene->goalPositionX = transform->positionX - transform->facingX * 8.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ - transform->facingZ * 8.0f;
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, 2.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);


	EntityID additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + cameraYOffset;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + cameraYOffset;
	cutscene->startLookAtZ = transform->positionZ;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX - transform->facingX * 256.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ - transform->facingZ * 256.0f;
	cutscene->goalPositionX = transform->positionX - transform->facingX * 5.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ - transform->facingZ * 5.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 2.0f, BeginCutscene, CutsceneTransition, 3.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + cameraYOffset;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + cameraYOffset;
	cutscene->startLookAtZ = transform->positionZ;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX - transform->facingX * 256.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ - transform->facingZ * 256.0f;
	cutscene->goalPositionX = transform->positionX - transform->facingX * 5.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ - transform->facingZ * 5.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 3.0f, BeginCutscene, CutsceneTransition, 3.5f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + cameraYOffset;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + cameraYOffset;
	cutscene->startLookAtZ = transform->positionZ;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX - transform->facingX * 256.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ - transform->facingZ * 256.0f;
	cutscene->goalPositionX = transform->positionX - transform->facingX * 5.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ - transform->facingZ * 5.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 3.5f, BeginCutscene, CutsceneTransition, 3.75f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY + cameraYOffset;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + cameraYOffset;
	cutscene->startLookAtZ = transform->positionZ;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX - transform->facingX * 256.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ - transform->facingZ * 256.0f;
	cutscene->goalPositionX = transform->positionX - transform->facingX * 5.0f;
	cutscene->goalPositionY = transform->positionY + cameraYOffset;
	cutscene->goalPositionZ = transform->positionZ - transform->facingZ * 5.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 3.75f, BeginCutscene, CutsceneTransition, 4.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transformPlayer->positionX;
	cutscene->goalLookAtY = transformPlayer->positionY;
	cutscene->goalLookAtZ = transformPlayer->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->positionY + 4.5f;
	cutscene->startLookAtZ = transform->positionZ;

	//Do one cutscene, then the other
	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();
	cutscene->startPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->startPositionY = transform->positionY + cameraYOffset;
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	cutscene->goalPositionX = transformPlayer->positionX + CAMERA_OFFSET_X;
	cutscene->goalPositionY = transformPlayer->positionY + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = transformPlayer->positionZ + CAMERA_OFFSET_Z;
	AddTimedEventComponentStartContinuousEnd(returnTransition, 3.6f, BeginCutscene, CutsceneTransition, 5.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, 5.1f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

void CutsceneFallStage1(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;
	float endTime = 5.0f;
	float newLevelTime = 4.5f;
	float fallTime = 3.0f;
	//Fall of point. -321.6, 133.14
	//Move camera to view point and background from a lower angle
	registry.RemoveComponent<HitboxComponent>(entity);//Remove GI constraints
	EntityID cutscene1 = registry.CreateEntity();
	CutsceneComponent* sidewaysCut = registry.AddComponent<CutsceneComponent>(cutscene1);
	sidewaysCut->mode = (CutsceneMode)0b111;
	CutsceneSetLookAt(cutscene1, -250.0f, 0.0f, 81.0f, -321.6f - 30.0f, 0.0f, 133.14f + 30.0f);
	CutsceneSetPosition(cutscene1, -250.0f, 90.0f, -80.0f, -300.0f, 90.0f, -20.0f);
	AddTimedEventComponentStartContinuousEnd(cutscene1, 0.0f, BeginPortalCutscene, CutsceneTransition, endTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Move character from portal to jump off point
	CutsceneComponent* playerWalk = registry.AddComponent<CutsceneComponent>(entity);
	playerWalk->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Character_Walk);
	CutsceneSetPosition(entity, -250.0f, 0.0f, 81.0f, -321.6f - 0.0f, 0.0f, 133.14f + 0.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginPortalCutscene, CutsceneTransition, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	
	AddTimedEventComponentStartContinuousEnd(entity, fallTime, CutscenePlayerFallDown, nullptr, endTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, endTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(entity, newLevelTime, EventLoadNextLevel, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
void CutsceneFallStage2(EntityID& entity, const int& index)
{
	//-334, 319

}

void CutscenePlayerFallDown(EntityID& entity, const int& index)
{
	//Find player component and create a timed event for falling downwards to a point. For use at beginning and end of stage.
	//Get player position, go downwards from start to goal.
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;
	float endTime = 2.5f;
	EntityID player = stateManager.player;
	TransformComponent* transform = registry.GetComponent<TransformComponent>(player);
	CutsceneComponent* downwardsFall = registry.AddComponent<CutsceneComponent>(player);
	AnimationComponent* anim = registry.AddComponent<AnimationComponent>(player);
	anim->aAnimIdx = 1;
	anim->aAnim = ANIMATION_IDLE;
	anim->aAnimTime = GetDeltaTime();
	downwardsFall->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Cutscene_Character_Fall | CutsceneMode::Cutscene_Accelerating);
	CutsceneSetPosition(entity, transform->positionX, transform->positionY + 1.0f, transform->positionZ, transform->positionX + transform->facingX*100.0f, transform->positionY - 300.0f, transform->positionZ + transform->facingZ * 100.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginPortalCutscene, CutsceneTransition, endTime, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, endTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void LoopCutscenePlayerFallInPlace(EntityID& entity, const int& index)
{
	//Loop every second

	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, CutscenePlayerFallInPlace, nullptr, 1.0f, LoopCutscenePlayerFallInPlace, 0, 2);
}

void CutscenePlayerFallInPlace(EntityID& entity, const int& index)
{
	//Find player component and create a timed event for falling in place. For use in shop.
	//Get player position. Stay in place
	EntityID player = stateManager.player;
	TransformComponent* transform = registry.GetComponent<TransformComponent>(player);
	CutsceneComponent* downwardsFall = registry.AddComponent<CutsceneComponent>(player);
	downwardsFall->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Cutscene_Character_Fall);
	CutsceneSetPosition(entity, transform->positionX, transform->positionY, transform->positionZ, transform->positionX, transform->positionY, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, 10.0f, EndCutscene, 0, 2);
}

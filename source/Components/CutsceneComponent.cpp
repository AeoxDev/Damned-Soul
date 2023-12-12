#include "CutsceneComponent.h"
#include "Registry.h"
#include "Camera.h"
#include "Components.h"
#include "EventFunctions.h"
#include "DeltaTime.h"
#include "States\StateManager.h"
#include "Camera.h"
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
	
	float posRandX = pointX + (rand() % 128) * 0.5f - 64.0f;
	float posRandY = (rand() % 256) * 0.2f;
	float posRandZ = pointZ - (rand() % 128) * 0.5f;
	
	


	cutscene->startLookAtX = pointX;
	cutscene->startLookAtY = 0.0f;
	cutscene->startLookAtZ = pointZ;
	cutscene->startPositionX = posRandX;
	cutscene->startPositionY = posRandY + 0.33f * CAMERA_OFFSET_Y;
	cutscene->startPositionZ = posRandZ;
	posRandX = posRandX + (rand() % 128) * 0.5f - 64.0f;
	posRandZ = posRandZ - (rand() % 128) * 0.5f;
	
	cutscene->goalPositionX = posRandX;
	cutscene->goalPositionY = 1.2f + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = posRandZ;
	float randX = pointX + (rand() % 128) * 0.5f - 64.0f;
	float randY = 0.0f;
	float randZ = pointZ +  (rand() % 128) * 0.5f - 64.0f;
	for (auto enemy : View<EnemyComponent, TransformComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(enemy);
		randX = transform->positionX;
		randZ = transform->positionZ;
		if (rand() % 16)
		{
			break;
		}

	}
	cutscene->goalLookAtX = randX;
	cutscene->goalLookAtY = randY;
	cutscene->goalLookAtZ = randZ;
	cutscene->mode = (CutsceneMode)(CutsceneMode::Cutscene_Linear | CutsceneMode::Transition_Position | Transition_LookAt | Cutscene_Camera);
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
	sidewaysCut->mode = (CutsceneMode)(Cutscene_Camera | CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Linear);
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
	returnCutscene->mode = (CutsceneMode)(Cutscene_Camera | CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Linear);
	CutsceneSetPosition(returnCamera, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z, CAMERA_OFFSET_X, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z);
	CutsceneSetLookAt(returnCamera, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);
	
	AddTimedEventComponentStartContinuousEnd(returnCamera, fallTime, BeginCutscene, CutsceneTransition, pauseTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//AddTimedEventComponentStart(returnCamera, pauseTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void StageIntroFallUnskippable(EntityID& entity, const int& index)
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
	sidewaysCut->mode = (CutsceneMode)(Cutscene_Camera | CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Linear);
	CutsceneSetLookAt(cutscene1, 0.0f, fallHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	CutsceneSetPosition(cutscene1, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(cutscene1, 0.0f, BeginShopCutscene, CutsceneTransition, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Fall down
	CutsceneComponent* fall = registry.AddComponent<CutsceneComponent>(entity);
	fall->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Cutscene_Character_Fall | CutsceneMode::Cutscene_Linear);
	CutsceneSetPosition(entity, 0.0f, fallHeight, 0.0f, 0.0f, 0.0f, 0.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginShopCutscene, CutsceneTransition, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);


	EntityID returnCamera = registry.CreateEntity();
	CutsceneComponent* returnCutscene = registry.AddComponent<CutsceneComponent>(returnCamera);
	returnCutscene->mode = (CutsceneMode)(Cutscene_Camera | CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Linear);
	CutsceneSetPosition(returnCamera, 0.0f, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z, CAMERA_OFFSET_X, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z);
	CutsceneSetLookAt(returnCamera, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f);

	AddTimedEventComponentStartContinuousEnd(returnCamera, fallTime, BeginShopCutscene, CutsceneTransition, pauseTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//AddTimedEventComponentStart(entity, pauseTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void SkeletonIntroScene(EntityID& entity, const int& index)
{
	//Look at skeletony!
	CutsceneNPCIntro1NoText(entity);
}

void HellhoundIntroScene(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	//Establish variables for the timings of the different parts of the cutscene
	float runningTime = 1.75f;					//Dog runs into frame for 2.0 seconds
	float attackingTime = runningTime + 1.0f;	//After that, dog does an attack for 1.0 seconds
		
	float bracingTime = attackingTime + 1.0f;	//Then we make camera pan up while dog braces for a second

	float breathTime = bracingTime + 1.0f;		//Breathe fire for a second

	float endReturnTime = breathTime + 1.0f;	//Wait a second before returning out of the cutscene
	float returnUnpause = endReturnTime + 0.1f; //Brief delay before unpausing the game

	//Get dog transform
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	transform->facingX = 0.33f;
	transform->facingY = 0.0f;
	transform->facingZ = -1.0f;
	
	float cameraXOffset = transform->scaleX * transform->facingX * 15.0f;
	float cameraYOffset = 1.f + transform->scaleY * 3.5f;
	float cameraZOffset = transform->scaleZ * transform->facingZ * 15.0f;

	//Create a still camera shot
	EntityID stillShot = registry.CreateEntity();
	CutsceneComponent* stillCameracutscene = registry.AddComponent<CutsceneComponent>(stillShot);
	stillCameracutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(stillShot, transform->positionX, cameraYOffset, transform->positionZ,
		transform->positionX, cameraYOffset, transform->positionZ);
	CutsceneSetPosition(stillShot, transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f);
	AddTimedEventComponentStartContinuousEnd(stillShot, 0.0f, BeginCutscene, CutsceneTransition, attackingTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Have dog run in from the side
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	float speedSquared = stats->GetSpeed() * stats->GetSpeed();
	float distX = runningTime * stats->GetSpeed();
	float dist = distX;
	CutsceneComponent* cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Walk | Cutscene_Linear | Transition_LookAt | Transition_Position);
	//Set Start and End for camera- and entity-movement
	CutsceneSetLookAt(entity, transform->facingZ, 0.0f, -transform->facingX, 
		transform->facingZ, 0.0f, -transform->facingX);
	CutsceneSetPosition(entity, transform->positionX + (-transform->facingZ * dist), 0.0f, transform->positionZ - (transform->facingX * dist),
		transform->positionX, 0.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, runningTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//MAKE DOG ATTACK
	//Step 1: Create new entity
	EntityID storedEntity = registry.CreateEntity();
	//Step 2: Put CutsceneComponent on the entity and set the mode(s)
	cutscene = registry.AddComponent<CutsceneComponent>(storedEntity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Attack | Cutscene_Linear | Transition_LookAt | Transition_Position);
	//Step 3: Set where the camera's look-vector and the entity's position is at the Start and End of the cutscene (I think)
	CutsceneSetLookAt(storedEntity, 
		transform->facingZ, 0.0f, -transform->facingX,
		transform->facingZ, 0.0f, -transform->facingX);
	CutsceneSetPosition(storedEntity, 
		transform->positionX, 0.0f, transform->positionZ,
		transform->positionX, 0.0f, transform->positionZ);
	//Step 4: Make timed event SPECIFICALLY using the StoredEnemyCutscene function (memcpy of relevant cutscene information from the last entity into storedEntity)
	AddTimedEventComponentStartContinuousEnd(storedEntity, runningTime, StoredEnemyCutscene, nullptr, attackingTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	
	//Make dog do dog noises
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, runningTime, PlayHellhoundIntroAttack, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);


	//MAKE CAMERA PAN UP AND DOG BRACE FOR SWOOSH
	EntityID panShot = registry.CreateEntity();
	CutsceneComponent* panCameraCutscene = registry.AddComponent<CutsceneComponent>(panShot);
	panCameraCutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | Transition_LookAt | Transition_Position);
	//Camera keeps looking at the dog
	CutsceneSetLookAt(panShot,
		transform->positionX, cameraYOffset, transform->positionZ,
		transform->positionX, cameraYOffset, transform->positionZ);
	//Camera moves upwards over the course of the cut
	CutsceneSetPosition(panShot,
		transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transform->positionX + transform->facingX * 60.0f, cameraYOffset * 12.0f, transform->positionZ + transform->facingZ * 60.0f);
	AddTimedEventComponentStartContinuousEnd(panShot, attackingTime, BeginCutscene, CutsceneTransition, bracingTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	storedEntity = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedEntity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_DogBrace | Cutscene_Linear | Transition_LookAt | Transition_Position);
	//Keep NOT moving the dog
	CutsceneSetLookAt(storedEntity,
		transform->facingZ, 0.0f, -transform->facingX,
		transform->facingZ, 0.0f, -transform->facingX);
	CutsceneSetPosition(storedEntity,
		transform->positionX, 0.0f, transform->positionZ,
		transform->positionX, 0.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedEntity, attackingTime, StoredEnemyCutscene, nullptr, bracingTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Play breath in sound effect
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, attackingTime, PlayHellhoundIntroBreathIn, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//MAKE DOG BREATHE FIRE
	storedEntity = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedEntity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_DogBreath | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedEntity,
		transform->facingZ, 0.0f, -transform->facingX,
		transform->facingZ, 0.0f, -transform->facingX);
	CutsceneSetPosition(storedEntity,
		transform->positionX, 0.0f, transform->positionZ,
		transform->positionX, 0.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedEntity, bracingTime, StoredEnemyDogBreathScene, nullptr, breathTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Play breath out sound effect
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, bracingTime, PlayHellhoundIntroBreathOut, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Return the camera and return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(returnTransition, transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX, 0.0f, transformPlayer->positionZ);
	CutsceneSetPosition(returnTransition, transform->positionX + transform->facingX * 60.0f,cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX + CAMERA_OFFSET_X, CAMERA_OFFSET_Y, transformPlayer->positionZ + CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(returnTransition, breathTime, BeginCutscene, CutsceneTransition, endReturnTime, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Unpause the game tchee
	AddTimedEventComponentStart(returnTransition, endReturnTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

	//Finally, remove the cutscene doggo
	AddTimedEventComponentStartContinuousEnd(entity, returnUnpause, nullptr, nullptr, returnUnpause, RemoveCutsceneEnemy, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	transform->positionY = -100.0f;
}

void ImpIntroScene(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);

	float cameraXOffset = transform->scaleX * transform->facingX * 15.0f;
	float cameraYOffset = 1.f + transform->scaleY * 2.5f;
	float cameraZOffset = transform->scaleZ * transform->facingZ * 15.0f;

	float teleportRight = 1.2f;
	float teleportLeft = 2.75f;
	float teleportFar = 4.0f;
	float teleportClose = 7.0f;
	float laugh = 8.0f;
	float beginReturnTime = laugh + 1.0f;
	float returnEndTime = beginReturnTime + 1.0f;
	float returnUnpause = returnEndTime + 0.1f;

	//Do the poofs
	AddTimedEventComponentStart(entity, teleportRight, ImpCutscenePoof, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 5);
	AddTimedEventComponentStart(entity, teleportLeft, ImpCutscenePoof, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 5);
	AddTimedEventComponentStart(entity, teleportFar, ImpCutscenePoof, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 5);
	AddTimedEventComponentStart(entity, teleportClose, ImpCutscenePoof, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 5);

	//Have camera still and show of imp.
	EntityID stillShot = registry.CreateEntity();
	CutsceneComponent* stillCameracutscene = registry.AddComponent<CutsceneComponent>(stillShot);
	stillCameracutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(stillShot, transform->positionX, cameraYOffset, transform->positionZ,
		transform->positionX, cameraYOffset, transform->positionZ);
	CutsceneSetPosition(stillShot, transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f);
	AddTimedEventComponentStartContinuousEnd(stillShot, 0.0f, BeginCutscene, CutsceneTransition, beginReturnTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Set imp away from the camera
	EntityID storedCutsceneSide = entity;
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneSide);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneSide, -1.0f, 0.0f, -1.0f,
		-1.0f, 0.0f, -1.0f);
	CutsceneSetPosition(storedCutsceneSide, 0.0f, -100.1f, 0.0f,
		0.0f, -100.0f, 0.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, teleportRight, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, teleportRight, PlayImpIntroTeleport, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 4);

	//Imp teleports into view on right side.
	storedCutsceneSide = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneSide);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneSide, -1.0f, 0.0f, -0.0f,
		-1.0f, 0.0f, -0.0f);
	CutsceneSetPosition(storedCutsceneSide, transform->positionX + 7.0f, 0.0f, transform->positionZ,
		transform->positionX + 7.0f, 0.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedCutsceneSide, teleportRight, StoredEnemyCutscene, nullptr, teleportLeft, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 4);
	//Play imp sound upon teleporting
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, teleportLeft, PlayImpIntroTeleport, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Imp teleports to left side
	storedCutsceneSide = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneSide);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneSide, 1.0f, 0.0f, 1.0f,
		1.0f, 0.0f, 1.0f);
	CutsceneSetPosition(storedCutsceneSide, transform->positionX - 7.0f, 0.0f, transform->positionZ,
		transform->positionX - 7.0f, 0.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedCutsceneSide, teleportLeft, StoredEnemyCutscene, nullptr, teleportFar, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, teleportFar, PlayImpIntroTeleport, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 4);

	//Imp teleports to middle
	//Turn around
	storedCutsceneSide = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneSide);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneSide, 1.0f, 0.0f, 1.0f,
		0.0f, 0.0f, -1.0f);
	CutsceneSetPosition(storedCutsceneSide, transform->positionX, 0.0f, transform->positionZ + 1.0f,
		transform->positionX, 0.0f, transform->positionZ + 1.0f);
	AddTimedEventComponentStartContinuousEnd(storedCutsceneSide, teleportFar, StoredEnemyCutscene, nullptr, teleportClose, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, teleportClose, PlayImpIntroTeleport, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 5);


	//Imp teleports to extreme close up and laughs
	storedCutsceneSide = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneSide);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneSide, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f);
	CutsceneSetPosition(storedCutsceneSide, transform->positionX + transform->facingX * 3.0f, 0.0f, transform->positionZ + transform->facingZ * 30.0f,
		transform->positionX +transform->facingX * 30.0f, 0.0f, transform->positionZ + transform->facingZ * 30.0f);
	AddTimedEventComponentStartContinuousEnd(storedCutsceneSide, teleportClose, StoredEnemyCutscene, nullptr, beginReturnTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//TeleportSound
	//Say voice line (SOUND)
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, laugh, PlayImpIntroLaugh, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Laugh

	//Return the camera
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(returnTransition, transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX, transformPlayer->positionY, transformPlayer->positionZ);
	CutsceneSetPosition(returnTransition, transform->positionX + transform->facingX * 60.0f, cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX + CAMERA_OFFSET_X, transformPlayer->positionY + CAMERA_OFFSET_Y, transformPlayer->positionZ + CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(returnTransition, beginReturnTime, BeginCutscene, CutsceneTransition, returnEndTime, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStart(returnTransition, returnUnpause, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStartContinuousEnd(entity, returnEndTime, nullptr, nullptr, returnUnpause, RemoveCutsceneEnemy, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	transform->positionY = -100.0f;
}

void ImpCutscenePoof(EntityID& entity, const int& index)
{
	//Do the poofs
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr && particle->metadataSlot != -1)
	{
		particle->Release();
	}
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	if (stats->GetDamage() < 20.f) //normal imp
	{
		registry.AddComponent<ParticleComponent>(entity, 100.0f, 100.0f, 25.0f, 0.0f, 2.0f, 1.0f, 32, VFX_PATTERN::SPAWN_IMP);
		AddTimedEventComponentStart(entity, 2.f, BossSpawnwaveEnd);
	}
	else
	{
		registry.AddComponent<ParticleComponent>(entity, 100.0f, 100.0f, 25.0f, 0.0f, 2.0f, 1.0f, 32, VFX_PATTERN::SPAWN_IMP_EMPOWERED);
		AddTimedEventComponentStart(entity, 2.f, BossSpawnwaveEnd);
	}
}

void EventRemoveParticle(EntityID& entity, const int& index)
{
	ParticleComponent* particle = registry.GetComponent<ParticleComponent>(entity);
	if (particle != nullptr && particle->metadataSlot != -1)
	{
		particle->Release();
	}
	registry.RemoveComponent<ParticleComponent>(entity);
}

void SplitBossIntroScene(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);
	transform->facingX = 1.0f;
	transform->facingZ = -0.0f;
	NormalizeFacing(transform);
	cutscene = registry.AddComponent<CutsceneComponent>(entity);

	float cameraYOffset = 3.0f;
	float cameraFinishLookat = 1.0f;
	float goToTime = 1.25f;
	float lookUpTime = 4.0f;
	float beginReturn = 5.0f;
	float endReturn = 6.1f;

	//Go to boss
	EntityID goForwardCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(goForwardCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Decelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(goForwardCutscene, transform->positionX, transform->positionY + cameraYOffset, transform->positionZ,
		transform->positionX, transform->positionY + cameraFinishLookat, transform->positionZ);
	//Do one cutscene, then the other
	float distance = 120.0f;
	float closeUp = 20.0f;
	CutsceneSetPosition(goForwardCutscene,transform->positionX + transform->facingX * distance, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * distance,
		transform->positionX + transform->facingX * closeUp, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * closeUp);
	AddTimedEventComponentStartContinuousEnd(goForwardCutscene, 0.0f, BeginCutscene, CutsceneTransition, goToTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Then look up at boss
	EntityID lookUp = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(lookUp);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Accelerating | CutsceneMode::Transition_LookAt);
	float lookUpPosition = transform->scaleY * (transform->positionY + 4.5f);
	CutsceneSetLookAt(lookUp, transform->positionX, transform->positionY + cameraFinishLookat, transform->positionZ,
		transform->positionX, transform->positionY + lookUpPosition, transform->positionZ);
	CutsceneSetPosition(lookUp, transform->positionX + transform->facingX * closeUp, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * closeUp,
		transform->positionX + transform->facingX * closeUp, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * closeUp);
	AddTimedEventComponentStartContinuousEnd(lookUp, goToTime, BeginCutscene, CutsceneTransition, lookUpTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Keep looking at boss

	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
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
	AddTimedEventComponentStartContinuousEnd(returnTransition, beginReturn, BeginCutscene, CutsceneTransition, endReturn, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(returnTransition, endReturn + 0.1f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

	cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear);
	AddTimedEventComponentStartContinuousEnd(entity, 0.f, nullptr, CutsceneTransition, endReturn + 0.1f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStartContinuousEnd(stateManager.player, 0.0f, nullptr, nullptr, beginReturn + 0.1f, PlayPlayerBossVoiceLine, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void Level8IntroScene(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);

	EntityID verticalSwipe = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(verticalSwipe);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 6.0f);
	cutscene->startLookAtZ = transform->positionZ;
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	float cameraXOffset = transform->facingX * 15.0f;
	float cameraYOffset = 0.0f;
	float cameraZOffset = transform->facingZ * 15.0f;
	//Vertical swipe
	cutscene->startPositionX = transform->positionX + cameraXOffset;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset + 6.0f);
	cutscene->startPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset;
	cutscene->goalPositionY = transform->scaleY * (transform->positionY + cameraYOffset + 2.0f);
	cutscene->goalPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(verticalSwipe, 0.0f, BeginCutscene, CutsceneTransition, 1.2f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	EntityID additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->scaleY * (transform->positionY + 2.0f);
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 2.0f);
	cutscene->startLookAtZ = transform->positionZ;

	cameraXOffset = transform->facingX * 25.0f;
	cameraYOffset = 4.5f;
	cameraZOffset = transform->facingZ * 25.0f;
	//Side to side
	cutscene->startPositionX = transform->positionX + cameraXOffset + cameraZOffset;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->startPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset - cameraXOffset;
	cutscene->goalPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->goalPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 1.2f, BeginCutscene, CutsceneTransition, 2.5f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Zoom into the enemy
	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->scaleY * (transform->positionY + 4.5f);
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 4.5f);
	cutscene->startLookAtZ = transform->positionZ;
	cameraYOffset = 4.5f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + transform->facingX * 40.0f;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 40.0f;
	cutscene->goalPositionX = transform->positionX + transform->facingX * 8.0f;
	cutscene->goalPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->goalPositionZ = transform->positionZ + transform->facingZ * 8.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 2.5f, BeginCutscene, CutsceneTransition, 4.0f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Accelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transformPlayer->positionX;
	cutscene->goalLookAtY = transformPlayer->positionY;
	cutscene->goalLookAtZ = transformPlayer->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 4.5f);
	cutscene->startLookAtZ = transform->positionZ;

	//Do one cutscene, then the other
	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();
	cutscene->startPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	cutscene->goalPositionX = transformPlayer->positionX + CAMERA_OFFSET_X;
	cutscene->goalPositionY = transformPlayer->positionY + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = transformPlayer->positionZ + CAMERA_OFFSET_Z;
	AddTimedEventComponentStartContinuousEnd(returnTransition, 4.4f, BeginCutscene, CutsceneTransition, 5.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(returnTransition, 5.5f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

}

//Have eye come in from side to side.
void EyeIntroScene(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	float runningTime = 1.0f;
	float waitingTime = runningTime + 0.6f;
	float runningTime2 = waitingTime + 1.0f;
	float waitingTime2 = runningTime2 + 0.6f;
	float flyDownTime = waitingTime2 + 1.0f;
	float waitingTime3 = flyDownTime + 0.9f;
	float beginReturnTime = waitingTime3;
	float returnEndTime = beginReturnTime + 1.0f;
	float returnUnpause = returnEndTime + 0.1f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);


	float cameraXOffset = transform->scaleX * transform->facingX * 15.0f;
	float cameraYOffset = 1.f + transform->scaleY * 3.5f;
	float cameraZOffset = transform->scaleZ * transform->facingZ * 15.0f;

	//Have camera still and let eye move from side to side
	EntityID stillShot = registry.CreateEntity();
	CutsceneComponent* stillCameracutscene = registry.AddComponent<CutsceneComponent>(stillShot);
	stillCameracutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(stillShot, transform->positionX, transform->positionY + cameraYOffset, transform->positionZ,
		transform->positionX, transform->positionY + cameraYOffset, transform->positionZ);
	CutsceneSetPosition(stillShot, transform->positionX + transform->facingX * 60.0f, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transform->positionX + transform->facingX * 60.0f, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * 60.0f);
	AddTimedEventComponentStartContinuousEnd(stillShot, 0.0f, BeginCutscene, CutsceneTransition, beginReturnTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Make eye run from right to left
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	float speedSquared = stats->GetSpeed() * stats->GetSpeed();
	float distX = runningTime * stats->GetSpeed();
	float dist = distX;
	float sideX = transform->facingZ * dist;
	float sideZ = -transform->facingX * dist;
	cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Walk | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(entity, sideX, 0.0f, sideZ,
		sideX, 0.0f, sideZ);
	CutsceneSetPosition(entity, transform->positionX - sideX, 0.0f, transform->positionZ - sideZ,
		transform->positionX + sideX, transform->positionY, transform->positionZ + sideZ);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, runningTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);


	//Make eye run from left to right

	EntityID storedCutsceneSide = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneSide);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Walk | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneSide, -sideX, 0.0f, -sideZ,
		-sideX, 0.0f, -sideZ);
	CutsceneSetPosition(storedCutsceneSide, transform->positionX + sideX, 0.0f, transform->positionZ + sideZ,
		transform->positionX - sideX, transform->positionY, transform->positionZ - sideZ);
	AddTimedEventComponentStartContinuousEnd(storedCutsceneSide, waitingTime, StoredEnemyCutscene, nullptr, runningTime2, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Make eye fly from above to close up shot camera
	EntityID storedCutsceneVertical = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedCutsceneVertical);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Decelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedCutsceneVertical, transform->facingX, 0.0f, transform->facingZ,
		transform->facingX, 0.0f, transform->facingZ);
	CutsceneSetPosition(storedCutsceneVertical, transform->positionX, 50.0f, transform->positionZ,
		transform->positionX, transform->positionY, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedCutsceneVertical, waitingTime2, StoredEnemyCutscene, nullptr, flyDownTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Continue the idle animation
	EntityID storedIdle = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedIdle);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Decelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedIdle, transform->facingX, 0.0f, transform->facingZ,
		transform->facingX, 0.0f, transform->facingZ);
	CutsceneSetPosition(storedIdle, transform->positionX, transform->positionY, transform->positionZ,
		transform->positionX, transform->positionY, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedIdle, flyDownTime, StoredEnemyCutscene, nullptr, beginReturnTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Return the camera
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(returnTransition, transform->positionX + transform->facingX * 60.0f, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX, transformPlayer->positionY, transformPlayer->positionZ);
	CutsceneSetPosition(returnTransition, transform->positionX + transform->facingX * 60.0f, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX + CAMERA_OFFSET_X, transformPlayer->positionY + CAMERA_OFFSET_Y, transformPlayer->positionZ + CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(returnTransition, beginReturnTime, BeginCutscene, CutsceneTransition, returnEndTime, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStart(returnTransition, returnUnpause, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

	AddTimedEventComponentStartContinuousEnd(entity, returnEndTime, nullptr, nullptr, returnUnpause, RemoveCutsceneEnemy, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void MinotaurIntroScene(EntityID& entity, const int& index)
{
	//Charge from far away
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	float chargingTime = 2.0f;
	float waitingTime = chargingTime + 0.3f;
	float beginReturnTime = waitingTime;
	float returnEndTime = beginReturnTime + 1.0f;
	float returnUnpause = returnEndTime + 0.1f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);


	float cameraXOffset = transform->scaleX * transform->facingX * 15.0f;
	float cameraYOffset = 1.f + transform->scaleY * 3.5f;
	float cameraZOffset = transform->scaleZ * transform->facingZ * 15.0f;

	//Have camera still and let minotaur charge it
	EntityID stillShot = registry.CreateEntity();
	CutsceneComponent* stillCameracutscene = registry.AddComponent<CutsceneComponent>(stillShot);
	stillCameracutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(stillShot, transform->positionX, transform->positionY + cameraYOffset, transform->positionZ + 1.0f,
		transform->positionX, transform->positionY + cameraYOffset, transform->positionZ + 1.0f);
	CutsceneSetPosition(stillShot, transform->positionX, transform->positionY + cameraYOffset, transform->positionZ,
		transform->positionX, transform->positionY + cameraYOffset, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(stillShot, 0.0f, BeginCutscene, CutsceneTransition, beginReturnTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Make minotaur charge at camera
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	float speedSquared = stats->GetSpeed() * stats->GetSpeed();
	float distZ = chargingTime * stats->GetSpeed() * 4.0f;
	float dist = distZ;
	float sideX = transform->facingZ * dist;
	float sideZ = -transform->facingX * dist;
	cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Attack | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(entity, 0.0f, 0.0f, -1.0f,
		0.0f, 0.0f, -1.0f);
	CutsceneSetPosition(entity, transform->positionX, 0.0f, transform->positionZ + dist,
		transform->positionX, transform->positionY, transform->positionZ + 5.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginCutscene, CutsceneTransition, chargingTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Minotaur noise
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, 0.1f, PlayMinotaurIntroCharge, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Return the camera
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(returnTransition, transform->positionX + transform->facingX * 60.0f, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX, transformPlayer->positionY, transformPlayer->positionZ);
	CutsceneSetPosition(returnTransition, transform->positionX + transform->facingX * 60.0f, transform->positionY + cameraYOffset, transform->positionZ + transform->facingZ * 60.0f,
		transformPlayer->positionX + CAMERA_OFFSET_X, transformPlayer->positionY + CAMERA_OFFSET_Y, transformPlayer->positionZ + CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(returnTransition, beginReturnTime, BeginCutscene, CutsceneTransition, returnEndTime, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStart(returnTransition, returnUnpause, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

	AddTimedEventComponentStartContinuousEnd(entity, returnEndTime, nullptr, nullptr, returnUnpause, RemoveCutsceneEnemy, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void FinalBossIntroCutscene(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;

	float playerToBossTime = 3.0f;
	float pause = 2.0f;
	float waitingTime = playerToBossTime + pause;
	float bossVoiceLineAndWalkTime = waitingTime;
	float bossFinishWalking = bossVoiceLineAndWalkTime + 8.0f;
	float returnToPlayerTime = bossFinishWalking;
	float playerReturned = returnToPlayerTime + 1.5f;
	float returnUnpause = returnToPlayerTime + 0.1f;

	TransformComponent* transform;
	CutsceneComponent* cutscene;
	transform = registry.GetComponent<TransformComponent>(entity);
	EntityID player = stateManager.player;
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(player);

	//Have player idle and face the boss
	EntityID playerIdle = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(playerIdle);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(playerIdle, 0.0f, 0.0f, 1.0f,
		0.0f, 0.0f, 1.0f);

	CutsceneSetPosition(playerIdle,
		0.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 0.0f);

	AddTimedEventComponentStartContinuousEnd(playerIdle, 0.0f, BeginShopCutscene, CutsceneTransition, returnUnpause, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Hide the boss'
	//Continue the idle animation
	EntityID storedIdle = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedIdle);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Decelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(storedIdle, transform->facingX, 0.0f, transform->facingZ,
		transform->facingX, 0.0f, transform->facingZ);
	CutsceneSetPosition(storedIdle, transform->positionX, transform->positionY + 200.0f, transform->positionZ,
		transform->positionX, transform->positionY + 200.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedIdle, 0.0f, StoredEnemyCutscene, nullptr, playerToBossTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Go from player to boss
	//Return to player
	EntityID toBoss = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(toBoss);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Decelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(toBoss, transformPlayer->positionX, transformPlayer->positionY, transformPlayer->positionZ,
		transform->positionX, transform->positionY, transform->positionZ);

	CutsceneSetPosition(toBoss,
		transformPlayer->positionX + CAMERA_OFFSET_X, transformPlayer->positionY + CAMERA_OFFSET_Y, transformPlayer->positionZ + CAMERA_OFFSET_Z,
		transform->positionX + CAMERA_OFFSET_X, transform->positionY + CAMERA_OFFSET_Y, transform->positionZ + CAMERA_OFFSET_Z);

	AddTimedEventComponentStartContinuousEnd(toBoss, 0.0f, BeginShopCutscene, CutsceneTransition, playerToBossTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Let boss jump down
	EntityID storedFall = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedFall);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_BossLanding | Cutscene_Linear | Transition_Position);
	CutsceneSetPosition(storedFall, transform->positionX, transform->positionY + 100.0f, transform->positionZ,
		transform->positionX, transform->positionY - 100.0f, transform->positionZ);
	AddTimedEventComponentStartContinuousEnd(storedFall, playerToBossTime, StoredEnemyCutscene, nullptr, waitingTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Slam sound upon landing (SOUND)
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, waitingTime - 0.5f*pause, PlayBossIntroSlam, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Camera from boss back to player
	EntityID fromBack = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(fromBack);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(fromBack, 0.0f, transform->positionY, 0.0f,
		0.0f, transform->positionY, 0.0f);
	CutsceneSetPosition(fromBack,
		16.f, 23.0f, 172.0f,
		-10.0f, 23.0f, 172.0f);
	AddTimedEventComponentStartContinuousEnd(fromBack, bossVoiceLineAndWalkTime, nullptr, CutsceneTransition, bossFinishWalking, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Walk towards player
	//Get speed
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	float length = 0.1f * stats->GetSpeed() * (bossFinishWalking - bossVoiceLineAndWalkTime);

	EntityID storedWalk = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(storedWalk);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Walk | Cutscene_Linear | Transition_Position);
	CutsceneSetPosition(storedWalk, transform->positionX, transform->positionY, transform->positionZ,
		transform->positionX, transform->positionY, transform->positionZ - length);
	AddTimedEventComponentStartContinuousEnd(storedWalk, bossVoiceLineAndWalkTime, StoredEnemyCutscene, nullptr, bossFinishWalking, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Say voice line (SOUND)
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, nullptr, bossVoiceLineAndWalkTime, PlayBossIntroVoiceLine, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Pan back to player
	//Return the camera
	//Return to player
	EntityID returnTransition = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(returnTransition, transform->positionX, transform->positionY, transform->positionZ * 0.5f,
		transformPlayer->positionX, transformPlayer->positionY, transformPlayer->positionZ);
	CutsceneSetPosition(returnTransition, -10.0f, 45.0f, 330.0f,
		transformPlayer->positionX + CAMERA_OFFSET_X, transformPlayer->positionY + CAMERA_OFFSET_Y, transformPlayer->positionZ + CAMERA_OFFSET_Z);
	AddTimedEventComponentStartContinuousEnd(returnTransition, returnToPlayerTime, BeginShopCutscene, CutsceneTransition, playerReturned, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(returnTransition, returnUnpause, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

	AddTimedEventComponentStartContinuousEnd(entity, playerReturned, nullptr, nullptr, returnUnpause, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStartContinuousEnd(stateManager.player, 0.0f, nullptr, nullptr, returnToPlayerTime + 0.1f, PlayPlayerBossVoiceLine, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void StoredEnemyCutscene(EntityID& entity, const int& index)
{
	//Find the cutscene enemy

	//Apply this function on the enemy.

	//Use total time - elapsed time for timer

	float time = GetTimedEventTotalTime(entity, index) - GetTimedEventElapsedTime(entity, index);

	CutsceneComponent* cutsceneTarget = registry.AddComponent<CutsceneComponent>(stateManager.cutsceneEnemy);
	CutsceneComponent* cutscene = registry.GetComponent<CutsceneComponent>(entity);
	memcpy(cutsceneTarget, cutscene, sizeof(CutsceneComponent));

	AddTimedEventComponentStartContinuousEnd(stateManager.cutsceneEnemy, 0.0f, nullptr, CutsceneTransition, time, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void StoredEnemyDogBreathScene(EntityID& entity, const int& index)
{
	//AAAAAAAAAAAAAAAA
	float time = GetTimedEventTotalTime(entity, index) - GetTimedEventElapsedTime(entity, index);

	CutsceneComponent* cutsceneTarget = registry.AddComponent<CutsceneComponent>(stateManager.cutsceneEnemy);
	CutsceneComponent* cutscene = registry.GetComponent<CutsceneComponent>(entity);
	memcpy(cutsceneTarget, cutscene, sizeof(CutsceneComponent));

	AddTimedEventComponentStartContinuousEnd(stateManager.cutsceneEnemy, 0.0f, SpawnCutsceneParticles, CutsceneTransition, time, DestroyCutsceneParticles, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
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

	EntityID verticalSwipe = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(verticalSwipe);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->positionY;
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 6.0f);
	cutscene->startLookAtZ = transform->positionZ;
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	float cameraXOffset = transform->facingX * 15.0f;
	float cameraYOffset = 0.0f;
	float cameraZOffset = transform->facingZ * 15.0f;
	//Vertical swipe
	cutscene->startPositionX = transform->positionX + cameraXOffset;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset + 6.0f);
	cutscene->startPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset;
	cutscene->goalPositionY = transform->scaleY * (transform->positionY + cameraYOffset + 2.0f);
	cutscene->goalPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(verticalSwipe, 0.0f, BeginCutscene, CutsceneTransition, 1.2f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN ,2);

	EntityID additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->scaleY * (transform->positionY + 2.0f);
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 2.0f);
	cutscene->startLookAtZ = transform->positionZ;

	cameraXOffset = transform->facingX * 25.0f;
	cameraYOffset = 4.5f;
	cameraZOffset = transform->facingZ * 25.0f;
	//Side to side
	cutscene->startPositionX = transform->positionX + cameraXOffset + cameraZOffset;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->startPositionZ =  transform->positionZ + transform->scaleZ * cameraZOffset;
	cutscene->goalPositionX = transform->positionX + cameraXOffset - cameraXOffset;
	cutscene->goalPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->goalPositionZ = transform->positionZ + transform->scaleZ * cameraZOffset;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 1.2f, BeginCutscene, CutsceneTransition, 2.5f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	
	//Zoom into the enemy
	additionalCutscene = registry.CreateEntity();
	cutscene = registry.AddComponent<CutsceneComponent>(additionalCutscene);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transform->positionX;
	cutscene->goalLookAtY = transform->scaleY * (transform->positionY + 4.5f);
	cutscene->goalLookAtZ = transform->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 4.5f);
	cutscene->startLookAtZ = transform->positionZ;
	cameraYOffset = 4.5f;
	//Do one cutscene, then the other
	cutscene->startPositionX = transform->positionX + transform->facingX * 40.0f;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 40.0f;
	cutscene->goalPositionX = transform->positionX + transform->facingX * 8.0f;
	cutscene->goalPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->goalPositionZ = transform->positionZ + transform->facingZ * 8.0f;
	AddTimedEventComponentStartContinuousEnd(additionalCutscene, 2.5f, BeginCutscene, CutsceneTransition, 4.0f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Return to player
	EntityID returnTransition = stateManager.player;
	cutscene = registry.AddComponent<CutsceneComponent>(returnTransition);
	TransformComponent* transformPlayer = registry.GetComponent<TransformComponent>(returnTransition);
	cutscene->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Accelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	cutscene->goalLookAtX = transformPlayer->positionX;
	cutscene->goalLookAtY = transformPlayer->positionY;
	cutscene->goalLookAtZ = transformPlayer->positionZ;
	cutscene->startLookAtX = transform->positionX;
	cutscene->startLookAtY = transform->scaleY * (transform->positionY + 4.5f);
	cutscene->startLookAtZ = transform->positionZ;

	//Do one cutscene, then the other
	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();
	cutscene->startPositionX = transform->positionX + transform->facingX * 4.0f;
	cutscene->startPositionY = transform->scaleY * (transform->positionY + cameraYOffset);
	cutscene->startPositionZ = transform->positionZ + transform->facingZ * 4.0f;
	cutscene->goalPositionX = transformPlayer->positionX + CAMERA_OFFSET_X;
	cutscene->goalPositionY = transformPlayer->positionY + CAMERA_OFFSET_Y;
	cutscene->goalPositionZ = transformPlayer->positionZ + CAMERA_OFFSET_Z;
	AddTimedEventComponentStartContinuousEnd(returnTransition, 4.4f, BeginCutscene, CutsceneTransition, 5.0f, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(returnTransition, 5.5f, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	//Make enemy idle and remove at end.
	cutscene = registry.AddComponent<CutsceneComponent>(entity);
	cutscene->mode = (CutsceneMode)(Cutscene_Character_Idle | Cutscene_Linear);
	AddTimedEventComponentStartContinuousEnd(entity, 0.f, nullptr, CutsceneTransition, 5.5f, RemoveCutsceneEnemy, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2 );
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
	sidewaysCut->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
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
void CutsceneFallStage(EntityID& entity, const int& index)
{
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;
	FallofComponent* fallof = registry.GetComponent<FallofComponent>(entity);//Player
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);//Player
	float portalX = transform->positionX;
	float portalZ = transform->positionZ;
	float fallofX = fallof->fallofX;
	float fallofZ = fallof->fallofZ;
	//Get player movement speed:
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	float speedSquared = stats->GetSpeed() * stats->GetSpeed();
	float distX = (portalX - fallofX);
	float distZ = (portalZ - fallofZ);
	float distSquared =  distX * distX + distZ * distZ;
	float dist = sqrtf(distSquared);
	float walkTime = sqrtf(distSquared) / sqrtf(speedSquared);
	float fallTime = walkTime + 2.0f;
	float newLevelTime = walkTime + 1.5f;
	float cameraZoom = 1.0f;
	float cameraToPortalTime = 0.5f;
	distX /= dist;
	distX *= 55.0f;
	distZ /= dist;
	distZ *= 55.0f;
	float portalHeight = 12.0f;
	//Move camera to view point and background from a lower angle
	registry.RemoveComponent<HitboxComponent>(entity);//Remove GI constraints
	EntityID cutscene1 = registry.CreateEntity();
	CutsceneComponent* sidewaysCut = registry.AddComponent<CutsceneComponent>(cutscene1);
	sidewaysCut->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Accelerating | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(cutscene1, portalX, 0.0f, portalZ, fallofX, 0.0f, fallofZ);
	CutsceneSetPosition(cutscene1, portalX, CAMERA_OFFSET_Y* cameraZoom, portalZ + CAMERA_OFFSET_Z * cameraZoom, portalX + distX, portalHeight, portalZ + distZ );
	AddTimedEventComponentStartContinuousEnd(cutscene1, 0.0f, BeginPortalCutscene, CutsceneTransition, cameraToPortalTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, 0.0f, ResetSquashStretch, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	//Stay at portal.
	EntityID cutscene2 = registry.CreateEntity();
	CutsceneComponent* portalShot = registry.AddComponent<CutsceneComponent>(cutscene2);
	portalShot->mode = (CutsceneMode)(Cutscene_Camera | Cutscene_Linear | CutsceneMode::Transition_LookAt | Transition_Position);
	CutsceneSetLookAt(cutscene2, fallofX, 0.0f, fallofZ, fallofX, 0.0f, fallofZ);
	CutsceneSetPosition(cutscene2, portalX + distX, portalHeight, portalZ + distZ, portalX + distX, portalHeight, portalZ + distZ);
	AddTimedEventComponentStartContinuousEnd(cutscene2, cameraToPortalTime, BeginPortalCutscene, CutsceneTransition, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	//Move character from portal to jump off point
	CutsceneComponent* playerWalk = registry.AddComponent<CutsceneComponent>(entity);
	playerWalk->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Transition_LookAt | CutsceneMode::Cutscene_Character_Walk);
	CutsceneSetPosition(entity, portalX, 0.0f, portalZ, fallofX - 0.0f, 0.0f, fallofZ + 0.0f);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginPortalCutscene, CutsceneTransition, walkTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);

	AddTimedEventComponentStartContinuousEnd(entity, walkTime, CutscenePlayerFallDown, nullptr, fallTime, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, fallTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(entity, newLevelTime, EventLoadNextLevel, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

void CutscenePlayerFallDown(EntityID& entity, const int& index)
{
	//Find player component and create a timed event for falling downwards to a point. For use at beginning and end of stage.
	//Get player position, go downwards from start to goal.
	TimedEventIgnoreGamespeed(true);
	gameSpeed = 0.0f;
	float endTime = 2.0f;
	float jumpForwardDist = 40.0f;
	float jumpDepth = 100.0f;
	EntityID player = stateManager.player;
	TransformComponent* transform = registry.GetComponent<TransformComponent>(player);
	CutsceneComponent* downwardsFall = registry.AddComponent<CutsceneComponent>(player);
	//AnimationComponent* anim = registry.AddComponent<AnimationComponent>(player);
	BlendAnimationComponent* anim = registry.AddComponent<BlendAnimationComponent>(player);
	anim->lower.aAnimIdx = 1;
	anim->lower.aAnim = ANIMATION_IDLE;
	anim->lower.aAnimTime = 0.5f;

	anim->upper.aAnimIdx = 1;
	anim->upper.aAnim = ANIMATION_IDLE;
	anim->upper.aAnimTime = 0.5f;

	downwardsFall->mode = (CutsceneMode)(CutsceneMode::Transition_Position | CutsceneMode::Cutscene_Character_Fall | CutsceneMode::Cutscene_Accelerating);
	CutsceneSetPosition(entity, transform->positionX, transform->positionY + 1.0f, transform->positionZ, transform->positionX + transform->facingX* jumpForwardDist, transform->positionY - jumpDepth, transform->positionZ + transform->facingZ * jumpForwardDist);
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, BeginPortalCutscene, CutsceneTransition, endTime, EndCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	AddTimedEventComponentStart(entity, endTime, SetGameSpeedDefault, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

void LoopCutscenePlayerFallInPlace(EntityID& entity, const int& index)
{
	//Loop every second

	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, CutscenePlayerFallInPlace, nullptr, 1.0f, LoopCutscenePlayerFallInPlace, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
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
	AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, CutsceneTransition, 10.0f, nullptr, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
}

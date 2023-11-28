#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "EventFunctions.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "EntityCreation.h"
#include "DeltaTime.h"
#include "Model.h"
#include "UIComponents.h"
#include "States\StateManager.h"

void LoadLevel1()
{
	float redMult = 1.0f;
	float greenMult = 1.0f;
	float blueMult = 1.0f;
	StageSetupVariables stageVars;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 1;
	stageVars.scaleX = 1.0f;
	stageVars.scaleY = 1.0f;
	stageVars.scaleZ = 1.0f;
	stageVars.offsetY = -0.1f;
	//stageVars.offsetX = 16.f;
	EntityID stage = SetUpStage(stageVars); //registry.CreateEntity();

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	SetGISpawnPosition(-0.0f, -0.0f);
	CreatePlayer(-0.0f, 0.0f, -0.0f, 80.0f, 100.0f, 20.0f, 10.0f, 1.0f, 1, 0.0f, 0.0, -1.0f);
	//			 posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	//SetupEnemy(EnemyType::skeleton, -224.f, 0.f, -32.f);
	//SetupEnemy(EnemyType::skeleton, -239.f, 0.f, -25.f);
	//SetupEnemy(EnemyType::skeleton, -210.f, 0.f, -40.f);

	//SetupEnemy(EnemyType::skeleton, -212.0f, 0.f, 72.f);
	//SetupEnemy(EnemyType::skeleton, -200.0f, 0.f, 69.f);
	//SetupEnemy(EnemyType::skeleton, -205.0f, 0.f, 88.f);

	//SetupEnemy(EnemyType::skeleton, -122.0f, 0.f, 61.f);
	//SetupEnemy(EnemyType::skeleton, -94.0f, 0.f, 54.f);

	SetupEnemy(EnemyType::hellhound, -224.f, 0.f, -32.f);
	SetupEnemy(EnemyType::hellhound, -239.f, 0.f, -25.f);
	SetupEnemy(EnemyType::hellhound, -210.f, 0.f, -40.f);
	SetupEnemy(EnemyType::hellhound, -212.0f, 0.f, 72.f);
	SetupEnemy(EnemyType::hellhound, -200.0f, 0.f, 69.f);
	SetupEnemy(EnemyType::hellhound, -205.0f, 0.f, 88.f);
	SetupEnemy(EnemyType::hellhound, -122.0f, 0.f, 61.f);
	SetupEnemy(EnemyType::hellhound, -94.0f, 0.f, 54.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::skeleton, -118.0f, 0.f, 96.f);
	//EntityID cutsceneEnemy = SetupEnemy(EnemyType::lucifer, 0.f, 0.f, 0.f, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 2.f, 2.f, 2.f);
	//registry.AddComponent<ParticleComponent>(stage, 5.0f, 10.f, 0.5f, 0.0f, 0.0f, 1.0f, SMOKE);
	//5 souls total
	
	/*char ctexture[] = "1-1C.png";
	char emptyTexture[] = "";
	AddStaticHazardTexture(stage, ctexture, emptyTexture, emptyTexture);*/

	
	//Finally set the collision boxes

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//registry.AddComponent<ParticleComponent>(stage, 10, 20, 5, 20, 0, 20, 20, FIRE); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	registry.AddComponent<ParticleComponent>(stage, 1, 0.5, 5, 20, 0, 20, 18, SPARK); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	//CreatePointLight(player, 1.0f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 100.0f, 10.0f);
	SetDirectionLight(1.0f, 0.8f, 0.6f, -1.6f, -3.0f, 1.0f);
	CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	//CreatePointLight(lightholder, 0.8f, 0.0f, 0.0f, 70.0f, 20.0f, 35.0f, 140.0f, 10.0f);
	CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	EntityID timeEntity = registry.CreateEntity(ENT_PERSIST_LEVEL);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(timeEntity);
	uiElement->Setup("TempShopTitle", "Time: 0", DSFLOAT2(0.8f, 0.8f));
	uiElement->SetAllVisability(false);

	UIGameTimeComponent* runtime = registry.AddComponent<UIGameTimeComponent>(timeEntity);

	stateManager.stage = stage;
	SetInPlay(true);
	//AddTimedEventComponentStart(cutsceneEnemy, 0.0f, Stage1IntroScene, 0, 1);
	Stage1IntroScene(cutsceneEnemy, 0);
}
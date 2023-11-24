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
	//			 posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	CreatePlayer(-0.0f, 0.0f, -0.0f, 80.0f, 100.0f, 20.0f,		10.0f,	1.0f,		 1,			0.0f, 0.0, -1.0f);

	SetupEnemy(EnemyType::skeleton, -204.0f, 0.f, 82.f);
	SetupEnemy(EnemyType::skeleton, -222.0f, 0.f, 28.f);
	SetupEnemy(EnemyType::skeleton, -230.0f, 0.f, -26.f);

	SetupEnemy(EnemyType::skeleton, -180.0f, 0.f, -24.f);
	SetupEnemy(EnemyType::skeleton, -121.0f, 0.f, 61.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::skeleton, -111.0f, 0.f, 104.f);

	//CutsceneFallStage1(stateManager.player, 0);


	

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

	//CreatePointLight(player, 1.0f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 100.0f, 10.0f);

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
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(cutsceneEnemy, 0.85f+0.3f+0.1f, Stage1IntroScene,CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
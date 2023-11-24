#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "Model.h"
#include "UIComponents.h"
#include "States\StateManager.h"
#include "EventFunctions.h"

void LoadLevel4()
{
	float redAdd = 0.15f;
	float greenAdd = 0.0f;
	float blueAdd = 0.0f;
	float redMult = 1.5f;
	float greenMult = 1.1f;
	float blueMult = 0.75f;

	StageSetupVariables stageVars;
	stageVars.ra = redAdd;
	stageVars.ga = greenAdd;
	stageVars.ba = blueAdd;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 4;
	EntityID stage = SetUpStage(stageVars);
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//**************************************************
	EntityID tempBoss = SetupEnemy(EnemyType::tempBoss, -58.f, 0.0f, 72.f);

	SetupEnemy(EnemyType::skeleton, -7.f, 0.0f, 123.f);
	SetupEnemy(EnemyType::skeleton, -180.f, 0.0f, -10.f);
	SetupEnemy(EnemyType::skeleton, 15.f, 0.0f, -3.f);
	

	TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
	stc->scaleX = 1.0f;
	stc->scaleZ = 1.0f;


	PointOfInterestComponent poic;
	poic.weight = 10.0f;



	


	EntityID mouse = registry.CreateEntity();
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	SetDirectionLight(1.0f, 0.666f, .466f, -1.6f, -3.0f, 1.0f);
	CreatePointLight(stage, 0.65f, 0.55f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, 0.38f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.38f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.38f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.38f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	
	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(tempBoss, 0.85f + 0.3f + 0.1f, Stage4IntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
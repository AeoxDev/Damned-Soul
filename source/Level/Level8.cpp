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
#include "UIButtonFunctions.h"

void LoadLevel8()
{
	float redAdd = 0.0f;
	float greenAdd = 0.0f;
	float blueAdd = 0.07f;
	float redMult = 1.0f;
	float greenMult = 1.0f;
	float blueMult = 1.0f;

	StageSetupVariables stageVars;
	stageVars.ra = redAdd;
	stageVars.ga = greenAdd;
	stageVars.ba = blueAdd;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 8;
	EntityID stage = SetUpStage(stageVars);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level8"); //Proximity hitbox (Added by Joaquin)

	if (SetupVFXTorches("LV8Torch.dss", true, false) == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV8Torch\nOr file is not written properly.");
	}

	EntityID mouse = registry.CreateEntity();


	SetupEnemyNavigationHelper(true); // This is for enemyfinder, ask Felix if you have a problem with it

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	if (SetupAllEnemies("LV8Enemies.dss") == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV8Enemies");
	}
	 
	stateManager.cutsceneEnemy = SetupEnemy(EnemyType::imp, -0.0f, 0.f, 115.f);
	ModelSkeletonComponent* model = registry.GetComponent<ModelSkeletonComponent>(stateManager.cutsceneEnemy);
	ReleaseModel(model->model);
	registry.RemoveComponent<ModelSkeletonComponent>(stateManager.cutsceneEnemy);

	//22 souls + 18 souls level 1,2 = 40 souls total before boss

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;


	//StageLights
	EntityID light = registry.CreateEntity();
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	EntityID lightholderFive = registry.CreateEntity();
	EntityID lightholderSix = registry.CreateEntity();

	float redLight = 0.5f;
	float greenLight = 0.0f;
	float blueLight = 0.5f;

	//CreatePointLight(light, redLight, greenLight, blueLight, 13.0f, 20.0f, 40.0f, 300.0f, 20.0f);// needs to be removed end of level
	//CreatePointLight(lightholder, redLight, greenLight, blueLight, 16.0f, 20.0f, -11.0f, 300.0f, 20.0f);
	//CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, -77.0f, 20.0f, -8.0f, 300.0f, 20.0f);
	//CreatePointLight(lightholderThree, redLight, greenLight, blueLight, 77.0f, 20.0f, 39.0f, 300.0f, 20.0f);
	//CreatePointLight(lightholderForth, redLight, greenLight, blueLight, -198.0f, 20.0f, -28.0f, 300.0f, 20.0f);
	//CreatePointLight(lightholderFive, redLight, greenLight, blueLight, -351.0f, 20.0f, -42.0f, 300.0f, 20.0f);
	//CreatePointLight(lightholderSix, redLight, greenLight, blueLight, -389.0f, 20.0f, 110.0f, 300.0f, 20.0f);

	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(stateManager.cutsceneEnemy, 0.85f + 0.3f + 0.1f, SkeletonIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);

}
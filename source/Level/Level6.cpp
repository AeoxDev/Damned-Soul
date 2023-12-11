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


void LoadLevel6()
{
	float redAdd = 0.0f;
	float greenAdd = 0.0f;
	float blueAdd = 0.1f;
	float redMult = 1.0f;
	float greenMult = 1.0f;
	float blueMult = 1.1f;

	StageSetupVariables stageVars;
	stageVars.ra = redAdd;
	stageVars.ga = greenAdd;
	stageVars.ba = blueAdd;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 6;
	EntityID stage = SetUpStage(stageVars);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level6"); //Proximity hitbox (Added by Joaquin)

	if (SetupVFXTorches("LV6Torch.dss", false, false) == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV6Torch\nOr file is not written properly.");
	}

	EntityID mouse = registry.CreateEntity();

	SetupEnemyNavigationHelper(false); // This is for enemyfinder, ask Felix if you have a problem with it

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	if (SetupAllEnemies("LV6Enemies.dss") == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV6Enemies");
	}
	


	stateManager.cutsceneEnemy = SetupEnemy(EnemyType::minotaur, -78.f, 1.f, 108.f, 0);

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	EntityID lightholderFive = registry.CreateEntity();
	EntityID lightholderSix = registry.CreateEntity();
	EntityID lightholderSeven = registry.CreateEntity();

	float redLight = 0.5f;
	float greenLight = 0.0f;
	float blueLight = 0.10f;

	CreatePointLight(lightholder, 0.8, 0.4, 0.0, 0.0f, 40.0f, -60.0f, 400.0f, 20.0f);
	CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, 12.0f, 10.0f, 74.0f, 500.0f, 20.0f);
	CreatePointLight(lightholderThree, redLight, greenLight, blueLight, -24.0f, 10.0f, 74.0f, 500.0f, 20.0f);
	CreatePointLight(lightholderForth, redLight, greenLight, blueLight, 32.0f, 10.0f, 146.0f, 500.0f, 20.0f);
	CreatePointLight(lightholderFive, redLight, greenLight, blueLight, -40.0f, 10.0f, 146.0f, 500.0f, 20.0f);
	CreatePointLight(lightholderSix, redLight, greenLight, blueLight, 32.0f, 10.0f, 216.0f, 500.0f, 20.0f);
	CreatePointLight(lightholderSeven, redLight, greenLight, blueLight, -40.0f, 10.0f, 216.0f, 500.0f, 20.0f);


	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(stateManager.cutsceneEnemy, 0.85f + 0.3f + 0.1f, MinotaurIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
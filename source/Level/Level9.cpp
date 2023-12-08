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

void LoadLevel9()
{
	float redAdd = 0.0f;
	float greenAdd = 0.0f;
	float blueAdd = 0.0f;
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
	stageVars.stageNr = 9;
	EntityID stage = SetUpStage(stageVars);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level9"); //Proximity hitbox (Added by Joaquin)

	if (SetupVFXTorches("LV9Torch.dss", false, true) == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV9Torch\nOr file is not written properly.");
	}

	EntityID mouse = registry.CreateEntity();

	stateManager.cutsceneEnemy = SetupEnemy(EnemyType::lucifer, 12.f, 0.f, 276.f);

	SetupEnemyNavigationHelper(false); // This is for enemyfinder, ask Felix if you have a problem with it

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//Mouse
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();

	//Light color
	float redLight = 0.0f;
	float greenLight = 0.5f;
	float blueLight = 1.0f;

	CreatePointLight(lightholder, redLight, greenLight, blueLight, -40.0f, 20.0f, 312.0f, 400.0f, 20.0f);
	CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, 50.0f, 20.0f, 300.0f, 400.0f, 20.0f);
	CreatePointLight(lightholderThree, 0.0, 0.5, 1.0f, -51.0f, 20.0f, 68.0f, 400.0f, 20.0f);


	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFallUnskippable, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(stateManager.cutsceneEnemy, 0.85f + 0.3f + 0.1f, FinalBossIntroCutscene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
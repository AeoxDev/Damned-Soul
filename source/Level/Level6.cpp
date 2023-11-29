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

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	 if (SetupAllEnemies("LV6Enemies.dss") == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV1Enemies");
	}

	stateManager.cutsceneEnemy = SetupEnemy(EnemyType::minotaur, -216.f, 1.f, -284.f, 0);

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);

	//Thing in the top right corner showing what level we're on
	/*UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(stage, DSFLOAT2(0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f), 3);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup("");*/

	/*UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DSFLOAT2(0.0f, 0.9f), DSFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");*/



	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;


	float redLight = 0.05f;
	float greenLight = 0.05f;
	float blueLight = 0.25f;


	CreatePointLight(stage, 0.4f, 0.6f, 0.15f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, redLight, greenLight, blueLight, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, redLight, greenLight, blueLight, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, redLight, greenLight, blueLight, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(stateManager.cutsceneEnemy, 0.85f + 0.3f + 0.1f, SkeletonIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
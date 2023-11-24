#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "EventFunctions.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "EntityCreation.h"
#include "DeltaTime.h"
#include "Model.h"
#include "UIComponents.h"
#include "States\StateManager.h"
#include "UIRenderer.h"

void LoadLevel3()
{
	float redAdd = 0.1f;
	float greenAdd = 0.0f;
	float blueAdd = 0.0f;
	float redMult = 1.4f;
	float greenMult = 1.2f;
	float blueMult = 0.8f;

	StageSetupVariables stageVars;
	stageVars.ra = redAdd;
	stageVars.ga = greenAdd;
	stageVars.ba = blueAdd;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 3;
	EntityID stage = SetUpStage(stageVars);
	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//Player
	//SetPlayerPosition(0.0, 0.0, 30.0f);
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth

	//Upper right corner:
	SetupEnemy(EnemyType::hellhound, -10.f, 0.f, 80.f);
	SetupEnemy(EnemyType::hellhound, -139.f, 0.f, 77.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::hellhound, -193.f, 0.f, 26.f);

	SetupEnemy(EnemyType::hellhound, -143.f, 0.f, 49.f);
	SetupEnemy(EnemyType::hellhound, -89.f, 0.f, 147.f);
	SetupEnemy(EnemyType::hellhound, -175.f, 0.f, 149.f);
	SetupEnemy(EnemyType::hellhound, -223.f, 0.f, 89.f);
	SetupEnemy(EnemyType::hellhound, -152.f, 0.f, -11.f);
	SetupEnemy(EnemyType::hellhound, -70.f, 0.f, -5.f);

	SetupEnemy(EnemyType::hellhound, -26.f, 0.f, 28.f);
	SetupEnemy(EnemyType::hellhound, 18.f, 0.f, 5.f);
	SetupEnemy(EnemyType::hellhound, -210.f, 0.f, 162.f);
	SetupEnemy(EnemyType::hellhound, -137.f, 0.f, 171.f);
	SetupEnemy(EnemyType::hellhound, -244.f, 0.f, 122.f);
	SetupEnemy(EnemyType::hellhound, -255.f, 0.f, 2.f);
	SetupEnemy(EnemyType::hellhound, -229.f, 0.f, -21.f);
	SetupEnemy(EnemyType::hellhound, -100.f, 0.f, 95.f);



	//Player Sounds
	//SoundComponent* scp = registry.AddComponent<SoundComponent>(player);
	//scp->Load(PLAYER);
	//registry.AddComponent<StatComponent>(player, 50.f, 20.0f, 50.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	//registry.AddComponent<PlayerComponent>(player);

	//registry.AddComponent<ControllerComponent>(player);

	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);

	//Thing in the top right corner showing what level we're on
	/*UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(stage, DSFLOAT2(0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f), 2);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup("");*/

	/*UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DSFLOAT2(0.0f, 0.9f), DSFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");*/



	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;


	CreatePointLight(stage, 0.6f, 0.6f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, 0.35f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.35f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.35f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.35f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	stateManager.stage = stage;
	SetInPlay(true);

	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(cutsceneEnemy, 0.85f + 0.3f + 0.1f, HellhoundIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
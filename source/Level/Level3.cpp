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
	SetupEnemy(EnemyType::skeleton, -18.f, 0.f, 101.f);
	SetupEnemy(EnemyType::skeleton, -37.f, 0.f, 101.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::hellhound, -13.f, 0.f, 80.f);

	//Under ribs
	SetupEnemy(EnemyType::hellhound, -188.f, 0.f, 24.f);
	SetupEnemy(EnemyType::hellhound, -181.f, 0.f, 33.f);
	SetupEnemy(EnemyType::hellhound, -194.f, 0.f, 13.f);
	SetupEnemy(EnemyType::skeleton, -202.f, 0.f, 39.f);

	//North of big bone 
	SetupEnemy(EnemyType::hellhound, -144.0f, 0.f, 168.f);
	SetupEnemy(EnemyType::imp, -150.f, 0.f, 199.f);
	SetupEnemy(EnemyType::imp, -184.f, 0.f, 166.f);
	SetupEnemy(EnemyType::imp, -162.f, 0.f, 154.f);

	//Doggo to guard the gate
	SetupEnemy(EnemyType::hellhound, -263, 0.f, 138.f);
	

	//Small arena:
	SetupEnemy(EnemyType::hellhound, -140.0f, 0.f, 80.f);
	SetupEnemy(EnemyType::imp, -158.f, 0.f, 94.f);
	SetupEnemy(EnemyType::skeleton, -158.f, 0.f, 72.f);
	//13 souls + 5 souls level 1 = 18 souls total


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

	SetDirectionLight(1.0f, 0.7f, .5f, -1.6f, -3.0f, 1.0f);
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
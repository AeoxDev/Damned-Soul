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

void LoadLevel2()
{
	float redAdd = 0.1f;
	float greenAdd = 0.0f;
	float blueAdd = 0.0f;
	float redMult = 1.2f;
	float greenMult = 1.0f;
	float blueMult = 1.0f;

	StageSetupVariables stageVars;
	stageVars.ra = redAdd;
	stageVars.ga = greenAdd;
	stageVars.ba = blueAdd;
	stageVars.rm = redMult;
	stageVars.gm = greenMult;
	stageVars.bm = blueMult;
	stageVars.stageNr = 2;
	EntityID stage = SetUpStage(stageVars); 
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level2"); //Proximity hitbox (Added by Joaquin)

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	
	//Enemies to the right:

	SetupEnemy(EnemyType::skeleton, 88.f, 0.f, 50.f, 0);
	SetupEnemy(EnemyType::skeleton, 68.0f, 0.f, 46.f, 1 );
	SetupEnemy(EnemyType::imp, 85.f, 0.f, 23.f, 1);

	//Enemies to the north:
	SetupEnemy(EnemyType::skeleton, 7.f, 0.f, 148.f, 1);
	SetupEnemy(EnemyType::skeleton, 28.f, 0.f, 145.f, 1);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::imp, 20.0f, 0.f, 160.0f, 1);

	//Enemies in ruins
	SetupEnemy(EnemyType::imp, -110.f, 0.f, 120.f, 1);
	SetupEnemy(EnemyType::skeleton, -84.f, 0.f, 154.f, 0);
	SetupEnemy(EnemyType::skeleton, -96.f, 0.f, 213.f, 0);
	SetupEnemy(EnemyType::skeleton, -143.f, 0.f, 127.f, 1);

	//Enemies on the left side:
	SetupEnemy(EnemyType::skeleton, -219.f, 0.f, 34.f, 1);
	SetupEnemy(EnemyType::skeleton, -235.0f, 0.f, 48.f, 0);
	SetupEnemy(EnemyType::imp, -237.f, 0.f, 18.f, 1);

	//One to guard the gate:
	SetupEnemy(EnemyType::skeleton, -230.f, 0.f, 214.f, 1);

	//22 souls + 18 souls level 1,2 = 40 souls total before boss

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

	float redLight = 0.35f;


	CreatePointLight(stage, 0.5f, 0.5f, 0.1f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, redLight, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, redLight, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, redLight, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, redLight, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(cutsceneEnemy, 0.85f + 0.3f + 0.1f, ImpIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

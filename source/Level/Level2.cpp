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

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth

	SetupEnemy(EnemyType::skeleton, -241.f, 0.f, 24.f);
	SetupEnemy(EnemyType::skeleton, -229.0f, 0.f, 117.f);
	SetupEnemy(EnemyType::skeleton, -236.0f, 0.f, 174.f);

	SetupEnemy(EnemyType::skeleton, -209.f, 0.f, 214.f);
	SetupEnemy(EnemyType::skeleton, -64.f, 0.f, 224.f);
	SetupEnemy(EnemyType::skeleton, 92.0f, 0.f, 107.0f);

	SetupEnemy(EnemyType::skeleton, 78.f, 0.f, 14.f);
	SetupEnemy(EnemyType::skeleton, -158.f, 0.f, 34.f);
	SetupEnemy(EnemyType::skeleton, -90.f, 0.f, 71.f);
	SetupEnemy(EnemyType::skeleton, -77.f, 0.f, 14.f);

	SetupEnemy(EnemyType::imp, -189.f, 0.f, 179.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::imp, -181.0f, 0.f, 67.f);
	SetupEnemy(EnemyType::imp, 46.f, 0.f, 145.f);
	SetupEnemy(EnemyType::imp, -67.f, 0.f, 148.f);

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

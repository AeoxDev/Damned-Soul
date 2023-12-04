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
	//float redAdd = 0.15f;
	//float greenAdd = 0.0f;
	//float blueAdd = 0.0f;
	//float redMult = 1.5f;
	//float greenMult = 1.1f;
	//float blueMult = 0.75f;
	// 
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
	stageVars.stageNr = 4;
	EntityID stage = SetUpStage(stageVars);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level4"); //Proximity hitbox (Added by Joaquin)
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//**************************************************
	EntityID tempBoss = SetupEnemy(EnemyType::tempBoss, -58.f, 0.0f, 72.f, 6969, 6969.f,
		6969.f, 6969.f, 6969.f, 6969.f,  2.f,
		 2.f, 2.f, 0.f,  0.f,  -1.f,  true, true,  true,
		true, true,  false);

	SetupEnemyNavigationHelper(false); // This is for enemyfinder, ask Felix if you have a problem with it

	TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
	stc->scaleX = 1.0f;
	stc->scaleZ = 1.0f;


	PointOfInterestComponent poic;
	poic.weight = 10.0f;

	EntityID mouse = registry.CreateEntity();
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	////StageLights
	//EntityID lightholder = registry.CreateEntity();
	//EntityID lightholderTwo = registry.CreateEntity();
	//EntityID lightholderThree = registry.CreateEntity();
	//EntityID lightholderForth = registry.CreateEntity();

	//CreatePointLight(lightholderThree, 0.6f, 0.f, 0.0f, 80.0f, 15.0f, -22.0f, 600.0f, 20.0f);// needs to be removed end of level
	//CreatePointLight(lightholder, 0.6f, 0.f, 0.0f, -50.0f, 15.0f, -70.0f, 600.0f, 20.0f);
	//CreatePointLight(lightholderTwo, 0.6f, 0.f, 0.0f, -190.0f, 15.0f, -70.0f, 600.0f, 20.0f);
	//CreatePointLight(lightholderForth, 0.6f, 0.f, 0.0f, -63.0f, 15.0f, 150.0f, 600.0f, 20.0f);

	//registry.AddComponent<ParticleComponent>(lightholder, 10, 0, 5.5, 0, 0, 1, 20, BOILING); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	//TransformComponent tComp;
	//tComp.positionX = -90.0f;
	//tComp.positionY = -7.0f;
	//tComp.positionZ = 38.0f;
	//registry.AddComponent<TransformComponent>(lightholder, tComp);

	//registry.AddComponent<ParticleComponent>(lightholderTwo, 10, 0, 5, 0,0,1, 8, FIRE); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	//TransformComponent ttComp;
	//ttComp.positionX = -90.0f;
	//ttComp.positionY = 1.0f;
	//ttComp.positionZ = -24.0f;
	//registry.AddComponent<TransformComponent>(lightholderTwo, ttComp);

	//registry.AddComponent<ParticleComponent>(lightholderThree, 10, 0, 5.5, 0, 0, 1, 10, BOILING); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	//TransformComponent tttComp;
	//tttComp.positionX = -60;
	//tttComp.positionY = -6;
	//tttComp.positionZ = 29;
	//registry.AddComponent<TransformComponent>(lightholderThree, tttComp);

	//registry.AddComponent<ParticleComponent>(lightholderForth, 10, 0, 5, 0, 0, 1, 8, FIRE); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	//TransformComponent fComp;
	//fComp.positionX = 181;
	//fComp.positionY = -1;
	//fComp.positionZ = 47;
	//registry.AddComponent<TransformComponent>(lightholderForth, fComp);

	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(tempBoss, 0.85f + 0.3f + 0.1f, SplitBossIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
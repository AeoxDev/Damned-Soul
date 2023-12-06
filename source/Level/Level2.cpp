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
	//float redAdd = 0.1f;
	//float greenAdd = 0.0f;
	//float blueAdd = 0.0f;
	//float redMult = 1.2f;
	//float greenMult = 1.0f;
	//float blueMult = 1.0f;
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
	stageVars.stageNr = 2;
	EntityID stage = SetUpStage(stageVars); 
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level2"); //Proximity hitbox (Added by Joaquin)

	EntityID mouse = registry.CreateEntity();


	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	
	if (SetupAllEnemies("LV2Enemies.dss") == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV2Enemies");
	}
	SetupEnemyNavigationHelper(false); // This is for enemyfinder, ask Felix if you have a problem with it

	//Enemies to the right:

	//SetupEnemy(EnemyType::skeleton, 88.f, 0.f, 50.f, 0);
	//SetupEnemy(EnemyType::skeleton, 68.0f, 0.f, 46.f, 1 );
	//SetupEnemy(EnemyType::imp, 85.f, 0.f, 23.f, 1);

	////Enemies to the north:
	//SetupEnemy(EnemyType::skeleton, 7.f, 0.f, 148.f, 1);
	//SetupEnemy(EnemyType::skeleton, 28.f, 0.f, 145.f, 1);
	stateManager.cutsceneEnemy = SetupEnemy(EnemyType::imp, -225.0f, 0.f, 234.0f, 0);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(stateManager.cutsceneEnemy);
	transform->facingZ = -1.0f;
	transform->facingX = 0.0f;

	////Enemies in ruins
	//SetupEnemy(EnemyType::imp, -110.f, 0.f, 120.f, 1);
	//SetupEnemy(EnemyType::skeleton, -84.f, 0.f, 154.f, 0);
	//SetupEnemy(EnemyType::skeleton, -96.f, 0.f, 213.f, 0);
	//SetupEnemy(EnemyType::skeleton, -143.f, 0.f, 127.f, 1);

	////Enemies on the left side:
	//SetupEnemy(EnemyType::skeleton, -219.f, 0.f, 34.f, 1);
	//SetupEnemy(EnemyType::skeleton, -235.0f, 0.f, 48.f, 0);
	//SetupEnemy(EnemyType::imp, -237.f, 0.f, 18.f, 1);

	////One to guard the gate:
	//SetupEnemy(EnemyType::skeleton, -230.f, 0.f, 214.f, 1);

	//22 souls + 18 souls level 1,2 = 40 souls total before boss

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	////StageLights
	//EntityID lightholder = registry.CreateEntity();


	//**// particels translates away on level reload //
	//CreateParticle(115, 14, -150, 10, 20, 6, 0, 0, 0, 20, 0);
	////**//
	//CreateParticle(95, 13, -134, 2, 0, 3, 0, 0, 0, 10, 9);

	/*CreateParticle(70, 20, -110, 10, 0, 7, 0, 0, 0, 30, 13);*/

	EntityID part = registry.CreateEntity();
	registry.AddComponent<ParticleComponent>(part, 10, 20, 6, 0, 0, 0, 20, SMOKE); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	TransformComponent ttComp;
	ttComp.positionX = -115;
	ttComp.positionY = 14;
	ttComp.positionZ = 150;
	registry.AddComponent<TransformComponent>(part, ttComp);

	EntityID partT = registry.CreateEntity();
	registry.AddComponent<ParticleComponent>(partT, 10, 20, 2.5f, 0, 0, 0, 8, FIRE); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	TransformComponent tttComp;
	tttComp.positionX = -118;
	tttComp.positionY = 5;
	tttComp.positionZ = 145;
	registry.AddComponent<TransformComponent>(partT, tttComp);


	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(stateManager.cutsceneEnemy, 0.85f + 0.3f + 0.04f, ImpIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}

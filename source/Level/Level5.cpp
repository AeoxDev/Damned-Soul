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

void LoadLevel5()
{
	//float redAdd = 0.0f;
	//float greenAdd = 0.1f;
	//float blueAdd = 0.0f;
	//float redMult = 1.0f;
	//float greenMult = 1.1f;
	//float blueMult = 1.0f;
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
	stageVars.stageNr = 5;
	EntityID stage = SetUpStage(stageVars);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("level5"); //Proximity hitbox (Added by Joaquin)


	SetupEnemyNavigationHelper(false); // This is for enemyfinder, ask Felix if you have a problem with it


	EntityID mouse = registry.CreateEntity();


	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	if (SetupAllEnemies("LV5Enemies.dss") == false)
	{
		//something went wrong, could not open file
		assert("Could not read file: LV5Enemies");
	}
	// 
	//SetupEnemy(EnemyType::skeleton, -25.f, 0.f, 50.f, 1); // make stronger skeleton
	//SetupEnemy(EnemyType::skeleton, 50.f, 0.f, -45.f, 1); // make stronger skeleton
	//SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 25.f, 1); // make stronger skeleton
	//SetupEnemy(EnemyType::skeleton, 30.f, 0.f, -25.f, 1); // make stronger skeleton
	//SetupEnemy(EnemyType::skeleton, -50.f, 0.f, 45.f, 1); // make stronger skeleton
	//EntityID cutsceneEnemy = SetupEnemy(EnemyType::skeleton, -40.f, 0.f, -45.f, 1); // make stronger skeleton
	//SetupEnemy(EnemyType::hellhound, 35.f, 0.f, 25.f, 3);
	//SetupEnemy(EnemyType::hellhound, 15.f, 0.f, -45.f, 2);
	//SetupEnemy(EnemyType::hellhound, 35.f, 1.f, 45.f, 2);
	//SetupEnemy(EnemyType::eye, -25.f, 1.f, -35.f, 1);
	//SetupEnemy(EnemyType::eye, -50.f, 1.f, 25.f, 1);
	//SetupEnemy(EnemyType::eye, -40.f, 1.f, 25.f, 1);
	//
	stateManager.cutsceneEnemy = SetupEnemy(EnemyType::eye, 65.f, 1.f, 135.f, 0);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(stateManager.cutsceneEnemy);
	transform->facingZ = -1.0f;
	transform->facingX = -0.5f;
	NormalizeFacing(transform);
	//22 souls + 18 souls level 1,2 = 40 souls total before boss

	/*registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());*/

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
	EntityID holder = registry.CreateEntity();

	CreatePointLight(lightholder, 0.6f, 0.1f, 0.0f, 82.0f, 10.0f, 185.0f, 700.0f, 20.0f);
	CreatePointLight(lightholderTwo, 0.6f, 0.1f, 0.0f, -318.0f, 10.0f, 163.0f, 200.0f, 20.0f);

	registry.AddComponent<ParticleComponent>(holder, 10, 0, 3.5, 0.0, 0.0, 0.0, 8, BOILING); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	TransformComponent tttComp;
	tttComp.positionX = -52.0;
	tttComp.positionY = 2.f;
	tttComp.positionZ = 136.0;
	registry.AddComponent<TransformComponent>(holder, tttComp);

	registry.AddComponent<ParticleComponent>(lightholder, 10, 0, 3.5,0.0, 0.0, 0.0, 19, BOILING); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	TransformComponent tComp;
	tComp.positionX = 5.0;
	tComp.positionY = 2.0f;
	tComp.positionZ = 135.0;
	registry.AddComponent<TransformComponent>(lightholder, tComp);

	registry.AddComponent<ParticleComponent>(lightholderTwo, 10, 0, 3.5, 0.0f, 0.0f, 0.0f, 9, BOILING); //(entity, float seconds, float radius, float size, float x, float y, float z,int amount, ComputeShaders pattern)
	TransformComponent ttComp;
	ttComp.positionX = -308.0f;
	ttComp.positionY = 8.0f;
	ttComp.positionZ = 160.0;
	registry.AddComponent<TransformComponent>(lightholderTwo, ttComp);

	//Add static hazards on the where player does not spawn
	//////const int nrHazards = 8;
	//////for (size_t i = 0; i < nrHazards; i++)
	//////{
	//////	SetUpHazard(HAZARD_ACID, 1.f, 0.f, 0.5f, 0.f, 0.2f, 1.2f, 0.2f, 1.5f);
	//////	/*bool succeded = false;
	//////	while (!succeded)
	//////	{
	//////		float randX = (float)(rand() % 100) - 50.0f;
	//////		float randZ = (float)(rand() % 100) - 50.0f;
	//////		if (randX * randX + randZ * randZ > 80)
	//////		{
	//////			float randScaleX = 5.0f + (float)((rand() % 100) * 0.1f);
	//////			float randScaleZ = 5.0f + (float)((rand() % 100) * 0.1f);
	//////			EntityID hazard = registry.CreateEntity();
	//////			ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
	//////			hazardModel->shared.colorAdditiveRed = 0.0f;
	//////			hazardModel->shared.colorAdditiveGreen = 0.5f;
	//////			hazardModel->shared.colorAdditiveBlue = 0.0f;
	//////			hazardModel->shared.colorMultiplicativeRed = 0.2f;
	//////			hazardModel->shared.colorMultiplicativeGreen = 1.2f;
	//////			hazardModel->shared.colorMultiplicativeBlue = 0.2f;
	//////			hazardModel->shared.gammaCorrection = 1.5f;
	//////			hazardModel->castShadow = false;
	//////			TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
	//////			hazardTransform->positionX = randX;
	//////			hazardTransform->positionY = 0.5f;
	//////			hazardTransform->positionZ = randZ;
	//////			hazardTransform->scaleX = randScaleX;
	//////			hazardTransform->scaleY = 1.0f;
	//////			hazardTransform->scaleZ = randScaleZ;
	//////			hazardTransform->facingX = cosf((float)rand());
	//////			hazardTransform->facingZ = sinf((float)rand());
	//////			AddStaticHazard(hazard, HAZARD_ACID);

	//////			succeded = true;
	//////		}
	//////	}*/
	//////}

	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(stateManager.cutsceneEnemy, 0.85f + 0.3f + 0.1f, EyeIntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
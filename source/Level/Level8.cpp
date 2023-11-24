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

void LoadLevel8()
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
	stageVars.stageNr = 8;
	EntityID stage = SetUpStage(stageVars);

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	SetupEnemy(EnemyType::empoweredImp, -164.f, 1.f, 185.f);
	SetupEnemy(EnemyType::empoweredImp, -110.f, 1.f, 185.f);
	SetupEnemy(EnemyType::empoweredImp, -278.f, 1.f, -166.f);
	SetupEnemy(EnemyType::empoweredImp, -1013.f, 1.f, 448.f);

	SetupEnemy(EnemyType::empoweredHellhound, -342.f, 1.f, -150.f);
	SetupEnemy(EnemyType::empoweredHellhound, -221.f, 1.f, -150.f);
	SetupEnemy(EnemyType::empoweredHellhound, -945.f, 1.f, -326.f);

	SetupEnemy(EnemyType::minotaur, -432.f, 1.f, 170.f);// make minitaur

	SetupEnemy(EnemyType::eye, -444.f, 1.f, 242.f);
	SetupEnemy(EnemyType::eye, -1045.f, 1.f, 327.f);

	EntityID cutsceneEnemy = SetupEnemy(EnemyType::tempBoss, -992.f, 0.f, 393.f);

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	SetupEnemy(EnemyType::empoweredSkeleton, -138.f, 0.f, 93.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -106.f, 0.f, 132.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -174.f, 0.f, 131.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -400.f, 0.f, 135.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -479.f, 0.f, 176.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -415.f, 0.f, 219.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -899.f, 0.f, 393.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -949.f, 1.f, 277.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -1037.f, 1.f, 284.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -1074.f, 1.f, 424.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -730.f, 1.f, 360.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -603.f, 1.f, 316.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -397.f, 1.f, 21.f);
	SetupEnemy(EnemyType::empoweredSkeleton, -283.f, 1.f, -38.f);
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


	float redLight = 0.05f;
	float greenLight = 0.05f;
	float blueLight = 0.25f;


	CreatePointLight(stage, 0.4f, 0.6f, 0.15f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, redLight, greenLight, blueLight, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, redLight, greenLight, blueLight, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, redLight, greenLight, blueLight, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	//Add static hazards on the where player does not spawn
	int nrHazards = 8;
	for (size_t i = 0; i < nrHazards; i++)
	{
		SetUpHazard(HAZARD_ACID, 1.f, 0.f, 0.5f, 0.f, 0.2f, 1.2f, 0.2f, 1.5f);
		/*bool succeded = false;
		while (!succeded)
		{
			float randX = (float)(rand() % 100) - 50.0f;
			float randZ = (float)(rand() % 100) - 50.0f;
			if (randX * randX + randZ * randZ > 80)
			{
				float randScaleX = 5.0f + (float)((rand() % 100) * 0.1f);
				float randScaleZ = 5.0f + (float)((rand() % 100) * 0.1f);
				EntityID hazard = registry.CreateEntity();
				ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
				hazardModel->shared.colorAdditiveRed = 0.0f;
				hazardModel->shared.colorAdditiveGreen = 0.5f;
				hazardModel->shared.colorAdditiveBlue = 0.0f;
				hazardModel->shared.colorMultiplicativeRed = 0.2f;
				hazardModel->shared.colorMultiplicativeGreen = 1.2f;
				hazardModel->shared.colorMultiplicativeBlue = 0.2f;
				hazardModel->shared.gammaCorrection = 1.5f;
				hazardModel->castShadow = false;
				TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
				hazardTransform->positionX = randX;
				hazardTransform->positionY = 0.5f;
				hazardTransform->positionZ = randZ;
				hazardTransform->scaleX = randScaleX;
				hazardTransform->scaleY = 1.0f;
				hazardTransform->scaleZ = randScaleZ;
				hazardTransform->facingX = cosf((float)rand());
				hazardTransform->facingZ = sinf((float)rand());
				AddStaticHazard(hazard, HAZARD_ACID);

				succeded = true;
			}
		}*/
	}
	nrHazards = 1;//Big ice sheet
	for (size_t i = 0; i < nrHazards; i++)
	{
		SetUpHazard(HAZARD_ICE, 5.f, .1f, .1f, .5f, .5f, 0.5f, 1.5f, 1.5f);
		/*bool succeded = false;
		float randX = (float)(rand() % 32) - 16.0f;
		float randZ = (float)(rand() % 32) - 16.0f;
		float randScaleX = 64.0f + (float)((rand() % 100) * 0.1f);
		float randScaleZ = 64.0f + (float)((rand() % 100) * 0.1f);
		EntityID hazard = registry.CreateEntity();
		ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
		hazardModel->shared.colorAdditiveRed = 0.1f;
		hazardModel->shared.colorAdditiveGreen = 0.1f;
		hazardModel->shared.colorAdditiveBlue = 0.5f;
		hazardModel->shared.colorMultiplicativeRed = 0.5f;
		hazardModel->shared.colorMultiplicativeGreen = 0.5f;
		hazardModel->shared.colorMultiplicativeBlue = 1.5f;
		hazardModel->shared.gammaCorrection = 1.5f;
		hazardModel->castShadow = false;
		TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
		hazardTransform->positionX = randX;
		hazardTransform->positionY = 0.4f;
		hazardTransform->positionZ = randZ;
		hazardTransform->scaleX = randScaleX;
		hazardTransform->scaleY = 1.0f;
		hazardTransform->scaleZ = randScaleZ;
		hazardTransform->facingX = cosf((float)rand());
		hazardTransform->facingZ = sinf((float)rand());
		AddStaticHazard(hazard, HAZARD_ICE);

		succeded = true;*/
	}

	stateManager.stage = stage;
	SetInPlay(true);
	AddTimedEventComponentStart(stateManager.player, 0.0f, StageIntroFall, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
	AddTimedEventComponentStart(cutsceneEnemy, 0.85f + 0.3f + 0.1f, Stage1IntroScene, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 1);
}
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
	float redAdd = 0.0f;
	float greenAdd = 0.1f;
	float blueAdd = 0.0f;
	float redMult = 1.0f;
	float greenMult = 1.1f;
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

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	SetupEnemy(EnemyType::skeleton, -25.f, 0.f, 50.f, 1); // make stronger skeleton
	SetupEnemy(EnemyType::skeleton, 50.f, 0.f, -45.f, 1); // make stronger skeleton
	SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 25.f, 1); // make stronger skeleton
	SetupEnemy(EnemyType::skeleton, 30.f, 0.f, -25.f, 1); // make stronger skeleton
	SetupEnemy(EnemyType::skeleton, -50.f, 0.f, 45.f, 1); // make stronger skeleton
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::skeleton, -40.f, 0.f, -45.f, 1); // make stronger skeleton
	SetupEnemy(EnemyType::hellhound, 35.f, 0.f, 25.f, 3);
	SetupEnemy(EnemyType::hellhound, 15.f, 0.f, -45.f, 2);
	SetupEnemy(EnemyType::hellhound, 35.f, 1.f, 45.f, 2);
	SetupEnemy(EnemyType::eye, -25.f, 1.f, -35.f, 1);
	SetupEnemy(EnemyType::eye, -50.f, 1.f, 25.f, 1);
	SetupEnemy(EnemyType::eye, -40.f, 1.f, 25.f, 1);
	

	Stage3IntroScene(cutsceneEnemy, 0);
	//22 souls + 18 souls level 1,2 = 40 souls total before boss

	/*registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());*/

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

	float redLight = 0.2f;
	float greenLight = 0.05f;
	float blueLight = 0.05f;

	SetDirectionLight(0.666f, 1.0f, .666f, -1.6f, -3.0f, 1.0f);
	CreatePointLight(stage, 0.4f, 0.6f, 0.15f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, redLight, greenLight, blueLight, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, redLight, greenLight, blueLight, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, redLight, greenLight, blueLight, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	//Add static hazards on the where player does not spawn
	const int nrHazards = 8;
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

	stateManager.stage = stage;
	SetInPlay(true);
}
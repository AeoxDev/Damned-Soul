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

	//Enemies to the right:

	SetupEnemy(EnemyType::skeleton, 88.f, 0.f, 50.f);
	SetupEnemy(EnemyType::skeleton, 68.0f, 0.f, 46.f);
	SetupEnemy(EnemyType::imp, 85.f, 0.f, 23.f);

	//Enemies to the north:
	SetupEnemy(EnemyType::skeleton, 7.f, 0.f, 148.f);
	SetupEnemy(EnemyType::imp, 28.f, 0.f, 145.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::imp, 20.0f, 0.f, 160.0f);

	//Enemies in ruins
	SetupEnemy(EnemyType::imp, -110.f, 0.f, 120.f);
	SetupEnemy(EnemyType::imp, -84.f, 0.f, 154.f);
	SetupEnemy(EnemyType::imp, -96.f, 0.f, 213.f);
	SetupEnemy(EnemyType::imp, -143.f, 0.f, 127.f);

	//Enemies on the left side:
	SetupEnemy(EnemyType::skeleton, -219.f, 0.f, 34.f);
	SetupEnemy(EnemyType::skeleton, -235.0f, 0.f, 48.f);
	SetupEnemy(EnemyType::imp, -237.f, 0.f, 18.f);

	//One to guard the gate:
	SetupEnemy(EnemyType::imp, -230.f, 0.f, 214.f);

	Stage3IntroScene(cutsceneEnemy, 0);
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

	SetDirectionLight(1.1f, 1.0f, .9f, -1.6f, -2.0f, 1.0f);
	CreatePointLight(stage, 0.5f, 0.5f, 0.1f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, redLight, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, redLight, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, redLight, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, redLight, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	//Add static hazards on the where player does not spawn
	const int nrHazards = 8;
	for (size_t i = 0; i < nrHazards; i++)
	{
		SetUpHazard(HAZARD_LAVA, 1.f, redAdd, greenAdd, blueAdd, redMult, greenMult, blueMult, 1.5f);
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
				hazardModel->shared.colorAdditiveRed = redAdd;
				hazardModel->shared.colorAdditiveGreen = greenAdd;
				hazardModel->shared.colorAdditiveBlue = blueAdd;
				hazardModel->shared.colorMultiplicativeRed = redMult;
				hazardModel->shared.colorMultiplicativeGreen = greenMult;
				hazardModel->shared.colorMultiplicativeBlue = blueMult;
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
				AddStaticHazard(hazard, HAZARD_LAVA);

				succeded = true;
			}
		}*/
	}


	stateManager.stage = stage;
	SetInPlay(true);
}

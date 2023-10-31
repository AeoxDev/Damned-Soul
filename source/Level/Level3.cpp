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

#include "UIRenderer.h"

void LoadLevel3()
{
	EntityID stage = registry.CreateEntity();
	EntityID playerUi = registry.CreateEntity();
	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	SetupEnemy(EnemyType::skeleton, -25.f, 0.f, 50.f);
	SetupEnemy(EnemyType::skeleton, 50.f, 0.f, -45.f);
	SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 25.f);
	SetupEnemy(EnemyType::skeleton, 30.f, 0.f, -25.f);
	SetupEnemy(EnemyType::skeleton, -50.f, 0.f, 45.f);
	SetupEnemy(EnemyType::skeleton, -40.f, 0.f, -45.f);
	SetupEnemy(EnemyType::skeleton, 35.f, 0.f, 25.f);
	SetupEnemy(EnemyType::hellhound, 45.f, 0.f, 50.f);
	SetupEnemy(EnemyType::hellhound, -35.f, 0.f, 45.f);
	SetupEnemy(EnemyType::hellhound, 15.f, 0.f, -45.f);
	SetupEnemy(EnemyType::eye, 35.f, 1.f, 45.f);
	SetupEnemy(EnemyType::eye, -25.f, 1.f, -35.f);
	SetupEnemy(EnemyType::eye, -50.f, 1.f, 25.f);
	//22 souls + 18 souls level 1,2 = 40 souls total before boss

	float redAdd = 0.0f;
	float greenAdd = 0.0f;
	float blueAdd = 0.1f;
	float redMult = 1.2f;
	float greenMult = 1.0f;
	float blueMult = 1.4f;

	ModelBonelessComponent* stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	stageModel->colorMultiplicativeRed = redMult;
	stageModel->colorMultiplicativeGreen = greenMult;
	stageModel->colorMultiplicativeBlue = blueMult;
	stageModel->colorAdditiveBlue = blueAdd;
	
	/*registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());*/

	// Stage (Default)
	registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);


	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->text.Setup("");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup("");

	//Thing in the top right corner showing what level we're on
	/*UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(stage, DSFLOAT2(0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f), 3);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup("");*/

	/*UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DSFLOAT2(0.0f, 0.9f), DSFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");*/

	RenderGeometryIndependentCollision(stage);

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;


	CreatePointLight(stage, 0.4f, 0.5f, 0.2f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.10f, 0.0f, 0.3f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.10f, 0.0f, 0.3f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	srand((unsigned)(GetDeltaTime() * 100000.0f));
	//Add static hazards on the where player does not spawn
	const int nrHazards = 8;
	for (size_t i = 0; i < nrHazards; i++)
	{
		bool succeded = false;
		while (!succeded)
		{
			float randX = (float)(rand() % 100) - 50.0f;
			float randZ = (float)(rand() % 100) - 50.0f;
			if (randX * randX + randZ * randZ > 80)
			{
				float randScaleX = 5.0f + (float)((rand() % 100) * 0.1f);
				float randScaleZ = 5.0f + (float)((rand() % 100) * 0.1f);
				EntityID hazard1 = CreateSquareStaticHazard("LavaPlaceholder.mdl", randX, 0.1f, randZ, randScaleX, 0.1f, randScaleZ,
					-0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f,
										3.0f, (float)rand(),
					redAdd - 0.1f, greenAdd - 0.1f, blueAdd +0.5f,
					redMult * 0.8f, greenMult * 0.8f, blueMult * 1.5f);
				succeded = true;
			}
		}
	}
}
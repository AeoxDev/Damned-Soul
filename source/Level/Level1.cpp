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
#include "UI/UIButton.h"
#include "UIButtonFunctions.h"

void LoadLevel1()
{
	
	EntityID stage = registry.CreateEntity();
	
	EntityID playerUi = registry.CreateEntity();
	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	CreatePlayer(0.0f, 0.0f, 0.0f, 3.0f, 100.0f, 20.0f, 50.0f, 5.0f, 1, 0.0f, 0.0, -1.0f);
	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	SetupEnemy(EnemyType::hellhound, -45.f, 0.f, -20.f);
	SetupEnemy(EnemyType::hellhound, 40.f, 0.f, -35.f);
	//SetupEnemy(EnemyType::skeleton, -30.f, 0.f, 45.f);
	//SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 45.f);
	//SetupEnemy(EnemyType::skeleton, -40.f, 0.f, 35.f);
	//SetupEnemy(EnemyType::eye, -0.f, 0.f, 25.f);
	// 
	//registry.AddComponent<ParticleComponent>(stage, 5.0f, 10.f, 0.5f, 0.0f, 0.0f, 1.0f, SMOKE);
	//5 souls total

	ModelBonelessComponent* stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	stageModel->colorMultiplicativeRed = 0.75f;
	stageModel->colorMultiplicativeGreen = 0.75f;
	stageModel->colorMultiplicativeBlue = 0.75f;
	//stageModel->colorAdditiveRed = 0.1f;
	
	// Stage (Default)
	TransformComponent *stageTransform = registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");	
	


	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->text.Setup("");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup("");

	//Thing in the top right corner showing what level we're on
	/*UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(stage, DSFLOAT2(0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f), 1);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup("");*/

	/*RelicHolderComponent* pRhc = registry.AddComponent<RelicHolderComponent>(player, "Relic Holder");

	UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DSFLOAT2(0.0f, 0.9f), DSFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");*/

	RenderGeometryIndependentCollision(stage);
	
	//Finally set the collision boxes

	
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//CreatePointLight(player, 1.0f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 100.0f, 10.0f);
	
	CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	//CreatePointLight(lightholder, 0.8f, 0.0f, 0.0f, 70.0f, 20.0f, 35.0f, 140.0f, 10.0f);
	CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	//CreateSpotLight(stage, 1.0f, 0.1f, 4.1f, 0.0f, 1.0f, 0.0f, 100.0f, 10.0f, 1.0f, -1.0f, 1.0f, 30.0f);
	//CreateSpotLight(skeleton, 100.0f, 0.1f, 4.1f, 0.0f, -10.0f, 0.0f, 100.0f, .1f, 1.0f, 1.0f, 1.0f, 80.0f);
	//CreateSpotLight(skeleton2, 1.0f, 0.1f, 4.1f, -30.0f, 10.0f, 0.0f, 100.0f, 100.0f, 1.0f, -10.0f, 1.0f, 30.0f);
	//RemoveLight(dog);
	//RemoveLight(stage);
	//RemoveLight(lightholder);
	//RemoveLight(lightholderTwo);
	//RemoveLight(lightholderThree);
	//RemoveLight(lightholderForth);

	//EntityID hazard1 = CreateRoundStaticHazard("PlaceholderScene.mdl",20.0f, 0.1f, -0.0f, 0.04f, 0.1f, 0.04f, 0.8f, 0.4f, 0.1f, 1.5f, 2.0f);
	//EntityID hazard2 = CreateRoundStaticHazard("PlaceholderScene.mdl",18.5f, 0.1f, -20.0f, 0.08f, 0.1f, 0.08f, 0.8f, 0.4f, 0.1f, 3.0f, 2.0f);
	srand((unsigned)(GetDeltaTime() * 100000.0f));

	//Add static hazards on the where player does not spawn
	//const int nrHazards = 3;
	//for (size_t i = 0; i < nrHazards; i++)
	//{
	//	bool succeded = false;
	//	while (!succeded)
	//	{
	//		float randX = (float)(rand() % 100) - 50.0f;
	//		float randZ = (float)(rand() % 100) - 50.0f;
	//		if (randX * randX + randZ * randZ > 80)
	//		{
	//			EntityID hazard1 = CreateSquareStaticHazard("PlaceholderScene.mdl", randX, 0.1f, randZ, 0.1f, 0.1f, 0.1f,
	//				-60.0f, -60.0f, 60.0f, -60.0f, 60.0f, 60.0f, -60.f, 60.f,
	//				0.8f, 0.5f, 0.1f, 3.0f, (float)rand());
	//			succeded = true;
	//		}
	//	}
	//}
}
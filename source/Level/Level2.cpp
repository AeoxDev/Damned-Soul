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

#include "UIRenderer.h"

void LoadLevel2()
{
	EntityID stage = registry.CreateEntity();
	EntityID skeleton = registry.CreateEntity();
	EntityID skeleton2 = registry.CreateEntity();
	EntityID skeleton3 = registry.CreateEntity();
	EntityID eye = registry.CreateEntity();
	EntityID dog = registry.CreateEntity();
	EntityID dog2 = registry.CreateEntity();
	EntityID playerUi = registry.CreateEntity();
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	SetupEnemy(skeleton, enemyType::skeleton, -25.f, 0.f, 50.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(skeleton2, enemyType::skeleton, 50.f, 0.f, -45.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(skeleton3, enemyType::skeleton, -50.f, 0.f, 25.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(dog, enemyType::hellhound, -35.f, 0.f, -25.f, 1.f, 150.f, 15.f, 10.f, 2.f, 3);
	SetupEnemy(dog2, enemyType::hellhound, 45.f, 0.f, -45.f, 1.f, 150.f, 15.f, 10.f, 2.f, 3);
	SetupEnemy(eye, enemyType::eye, 35.f, 1.f, -25.f, 1.f, 60.f, 8.f, 10.f, 5.f, 2);
	//11 souls + 6 souls level 1 = 17 souls total before boss

	registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	/*registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());*/

	// Stage (Default)
	registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");


	registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	///*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);


	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->text.Setup("");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup("");

	//Thing in the top right corner showing what level we're on
	UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(stage, DirectX::XMFLOAT2(0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f), 2);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup("");

	/*UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DirectX::XMFLOAT2(0.0f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");*/

	RenderGeometryIndependentCollision(stage);

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;


	CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

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
				EntityID hazard1 = CreateSquareStaticHazard("PlaceholderScene.mdl", randX, 0.1f, randZ, 0.1f, 0.1f, 0.1f,
					-60.0f, -60.0f, 60.0f, -60.0f, 60.0f, 60.0f, -60.f, 60.f,
					0.8f, 0.5f, 0.1f, 3.0f, (float)rand());
				succeded = true;
			}
		}
	}
}
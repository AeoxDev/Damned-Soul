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

void LoadLevel1()
{
	//SetDirectionLight(0.8f, 0.7f, 0.6f, -1.0f, -1.0f, 1.0f);
	//Doggo
	EntityID gameLevel = registry.CreateEntity();

	EntityID stage = registry.CreateEntity();
	EntityID player = registry.CreateEntity();
	EntityID skeleton = registry.CreateEntity();
	EntityID skeleton2 = registry.CreateEntity();
	EntityID eye = registry.CreateEntity();
	EntityID dog = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	

	SetupEnemy(skeleton, enemyType::skeleton, 5.f, 0.f, 7.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(skeleton2, enemyType::skeleton, 10.f, 0.f, 5.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(dog, enemyType::hellhound, -5.f, 0.f, -5.f, 1.f, 100.f, 15.f, 5.f, 2.f, 1);
	SetupEnemy(eye, enemyType::eye, 15.f, 0.f, 15.f, 1.f, 100.f, 15.f, 5.f, 2.f, 1);


	registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());
	

	
	// Stage (Default)
	registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");
	
	// Player (Default)
	TransformComponent* playerTransform = registry.AddComponent<TransformComponent>(player);
	playerTransform->facingZ = 1.0f;
	playerTransform->mass = 3.0f;
	
	

	registry.AddComponent<StatComponent>(player, 50.f, 20.0f, 100.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	registry.AddComponent<PlayerComponent>(player);

	

	registry.AddComponent<ControllerComponent>(player);

	/*ParticleComponent* particComp = */registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	registry.AddComponent<PointOfInterestComponent>(player, poic);


	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->text.Setup(L"");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup(L"");

	//Thing in the top right corner showing what level we're on (Christian why is the valueAsString thing here?) 
	//															(Because it was using the scene int parameter at the time)

	UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, DirectX::XMFLOAT2(0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f), 1);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup(L"");


	  
	RelicHolderComponent* pRhc = registry.AddComponent<RelicHolderComponent>(player, "Relic Holder");

	UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DirectX::XMFLOAT2(0.0f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");

	RenderGeometryIndependentCollision(stage);
	
	//Finally set the collision boxes

	SetupPlayerCollisionBox(player, 1.0f);
	MouseComponentAddComponent(player);
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
	const int nrHazards = 8;
	for (size_t i = 0; i < nrHazards; i++)
	{
		bool succeded = false;
		while (!succeded)
		{
			float randX = (float)(rand() % 120) - 60.0f;
			float randZ = (float)(rand() % 120) - 60.0f;
			if (randX * randX + randZ * randZ > 100)
			{
				EntityID hazard1 = CreateSquareStaticHazard("PlaceholderScene.mdl", randX, 0.1f, randZ, 0.1f, 0.1f, 0.1f,
					-60.0f, -60.0f, 60.0f, -60.0f, 60.0f, 60.0f, -60.f, 60.f,
					0.8f, 0.5f, 0.1f, 3.0f, (float)rand());
				succeded = true;
			}
		}
	}
}
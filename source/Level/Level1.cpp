#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "EventFunctions.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"

void LoadLevel1()
{
	//Doggo
	EntityID gameLevel = registry.CreateEntity();

	EntityID stage = registry.CreateEntity();
	EntityID player = registry.CreateEntity();
	EntityID skeleton = registry.CreateEntity();
	EntityID skeleton2 = registry.CreateEntity();
	EntityID eye = registry.CreateEntity();
	EntityID dog = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();
	EntityID mouse = registry.CreateEntity();

	

	SetupEnemy(skeleton, enemyType::skeleton, 5.f, 0.f, 7.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(skeleton2, enemyType::skeleton, 10.f, 0.f, 5.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
	SetupEnemy(dog, enemyType::hellhound, -5.f, 0.f, -5.f, 1.f, 100.f, 10.f, 5.f, 2.f, 1);
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
	
	

	registry.AddComponent<StatComponent>(player, 1250.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	registry.AddComponent<PlayerComponent>(player);

	

	registry.AddComponent<ControllerComponent>(player);

	/*ParticleComponent* particComp = */registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	registry.AddComponent<PointOfInterestComponent>(player, poic);


	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiHpC->image.Setup("ExMenu/FullHealth.png");
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
	//MouseComponentAddComponent(mouse);
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;
	//AddTimedEventComponentStartContinousEnd(player, player, 3.0f, RandomPosition, dog, RandomPosition, skeleton, 6.0f, RandomPosition);
	//AddTimedEventComponentStartContinousEnd(skeleton, skeleton, 7.0f, RandomPosition, skeleton2, RandomPosition, dog, 15.0f, RandomPosition);

}
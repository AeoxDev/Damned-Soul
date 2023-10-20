#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "EventFunctions.h"

void LoadLevel1()
{
	//Doggo
	EntityID gameLevel = registry.CreateEntity();
	EntityID dog = registry.CreateEntity();
	EntityID stage = registry.CreateEntity();
	EntityID player = registry.CreateEntity();
	EntityID skeleton = registry.CreateEntity();
	EntityID skeleton2 = registry.CreateEntity();
	EntityID eye = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	registry.AddComponent<ModelBonelessComponent>(dog, LoadModel("HellhoundDummy_PH.mdl"));
	registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());
	registry.AddComponent<ModelBonelessComponent>(skeleton, LoadModel("SkeletonOneDymmy.mdl"));
	registry.AddComponent<ModelBonelessComponent>(skeleton2, LoadModel("SkeletonOneDymmy.mdl"));
	registry.AddComponent<ModelBonelessComponent>(eye, LoadModel("FlyingEyeDymmy.mdl"));

	// Dog
	TransformComponent dogTransformComponent;
	dogTransformComponent.facingX = 1.0f; dogTransformComponent.positionX = 20.0f; dogTransformComponent.facingX = 1.0f;
	dogTransformComponent.mass = 9.0f;
	registry.AddComponent<TransformComponent>(dog, dogTransformComponent);
	
	// Stage (Default)
	registry.AddComponent<TransformComponent>(stage);
	
	// Player (Default)
	TransformComponent* playerTransform = registry.AddComponent<TransformComponent>(player);
	playerTransform->facingZ = 1.0f;
	playerTransform->mass = 3.0f;
	
	// First skeleton
	TransformComponent fsTransformComponent;
	fsTransformComponent.positionZ = 20.0f;
	registry.AddComponent<TransformComponent>(skeleton, fsTransformComponent);
	
	// Second skeleton
	TransformComponent ssTransformComponent;
	ssTransformComponent.positionZ = 15.0f;
	registry.AddComponent<TransformComponent>(skeleton2, ssTransformComponent);

	// Eye 
	TransformComponent eyeTransformComponent;
	eyeTransformComponent.positionX = 15.0f;
	eyeTransformComponent.positionZ = 15.0f;
	registry.AddComponent<TransformComponent>(eye, eyeTransformComponent);

	registry.AddComponent<StatComponent>(player, 1250.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	registry.AddComponent<PlayerComponent>(player);

	registry.AddComponent<StatComponent>(dog, 50.f, 20.f, 25.f, 5.f);
	registry.AddComponent<EnemyComponent>(dog, 1);

	registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
	registry.AddComponent<EnemyComponent>(skeleton, 2);

	registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);
	registry.AddComponent<EnemyComponent>(skeleton2, 2);

	registry.AddComponent<StatComponent>(eye, 50.f, 15.f, 37.f, 5.f);
	registry.AddComponent<EnemyComponent>(eye, 1);

	registry.AddComponent<ControllerComponent>(player);

	/*ParticleComponent* particComp = */registry.AddComponent<ParticleComponent>(particle, 10.f, 120.f, 0.5f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	registry.AddComponent<PointOfInterestComponent>(player, poic);
	//registry.AddComponent<PointOfInterestComponent>(dog);
	//registry.AddComponent<PointOfInterestComponent>(skeleton);
	//registry.AddComponent<PointOfInterestComponent>(skeleton2);
	//registry.AddComponent<PointOfInterestComponent>(eye);

	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiHpC->image.Setup("ExMenu/FullHealth.png");
	pcUiHpC->text.Setup(L"");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup(L"");

	//Thing in the top right corner showing what level we're on (Christian why is the valueAsString thing here?) 
	//															(Because it was using the scene int parameter at the time)
	//std::string valueAsString = "1";
	//std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());
	UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, DirectX::XMFLOAT2(0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f), 1);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup(L"");

	registry.AddComponent<HellhoundBehaviour>(dog);
	registry.AddComponent<SkeletonBehaviour>(skeleton);
	registry.AddComponent<SkeletonBehaviour>(skeleton2);
	registry.AddComponent<EyeBehaviour>(eye);

	RelicHolderComponent* pRhc = registry.AddComponent<RelicHolderComponent>(player, "Relic Holder");

	UIPlayerRelicsComponent* pcUiRc = registry.AddComponent<UIPlayerRelicsComponent>(player, DirectX::XMFLOAT2(0.0f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), 0);
	pcUiRc->baseImage.Setup("TempRelicHolder2.png");

	RenderGeometryIndependentCollision(stage);
	
	//Finally set the collision boxes

	SetupPlayerCollisionBox(player, 1.0f);
	SetupEnemyCollisionBox(skeleton, 0.9f);
	SetupEnemyCollisionBox(skeleton2, 0.9f);
	SetupEnemyCollisionBox(dog, 1.0f);
	SetupEnemyCollisionBox(eye, 1.0f);
	//AddTimedEventComponentStartContinousEnd(player, player, 3.0f, RandomPosition, dog, RandomPosition, skeleton, 6.0f, RandomPosition);
	//AddTimedEventComponentStartContinousEnd(skeleton, skeleton, 7.0f, RandomPosition, skeleton2, RandomPosition, dog, 15.0f, RandomPosition);
}
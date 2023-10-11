#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "EventFunctions.h"

void LoadLevel1()
{
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"This is the HUD!", { 0.0f, 0.6f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Press ECS to return to main menu!", { 0.0f, 0.4f }));
	//Doggo
	EntityID gameLevel = registry.CreateEntity();
	EntityID dog = registry.CreateEntity();
	EntityID stage = registry.CreateEntity();
	EntityID player = registry.CreateEntity();
	EntityID skeleton = registry.CreateEntity();
	EntityID skeleton2 = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	/*ModelBonelessComponent* dogCo = */registry.AddComponent<ModelBonelessComponent>(dog, LoadModel("HellhoundDummy_PH.mdl"));
	/*ModelBonelessComponent* stageCo = */registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	/*ModelSkeletonComponent* pmc = */registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	/*AnimationComponent* pac = */registry.AddComponent<AnimationComponent>(player, AnimationComponent());
	/*ModelBonelessComponent* skelCo = */registry.AddComponent<ModelBonelessComponent>(skeleton, LoadModel("SkeletonOneDymmy.mdl"));
	/*ModelBonelessComponent* skelCo2 = */registry.AddComponent<ModelBonelessComponent>(skeleton2, LoadModel("SkeletonOneDymmy.mdl"));

	// Dog
	TransformComponent dogTransformComponent;
	dogTransformComponent.facingX = 1.0f; dogTransformComponent.positionX = 20.0f; dogTransformComponent.facingX = 1.0f;
	/*TransformComponent* dtc = */registry.AddComponent<TransformComponent>(dog, dogTransformComponent);
	// Stage (Default)
	/*TransformComponent* stc = */registry.AddComponent<TransformComponent>(stage);
	// Player (Default)
	/*TransformComponent* ptc = */registry.AddComponent<TransformComponent>(player);
	// First skeleton
	TransformComponent fsTransformComponent;
	fsTransformComponent.positionZ = 20.0f;
	/*TransformComponent* skeltc = */registry.AddComponent<TransformComponent>(skeleton, fsTransformComponent);
	// Second skeleton
	TransformComponent ssTransformComponent;
	ssTransformComponent.positionZ = 15.0f;
	/*TransformComponent* skeltc2 = */registry.AddComponent<TransformComponent>(skeleton2, ssTransformComponent);



	/*StatComponent* ps = */registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 10.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	/*PlayerComponent* pc = */registry.AddComponent<PlayerComponent>(player);

	/*StatComponent* ds = */registry.AddComponent<StatComponent>(dog, 50.f, 20.f, 25.f, 5.f);
	/*EnemyComponent* ec1 = */registry.AddComponent<EnemyComponent>(dog, 1);

	/*StatComponent* ss = */registry.AddComponent<StatComponent>(skeleton, 100.f, 10.f, 25.f, 5.f);
	/*EnemyComponent* ec2 = */registry.AddComponent<EnemyComponent>(skeleton, 2);

	/*StatComponent* ss2 = */registry.AddComponent<StatComponent>(skeleton2, 100.f, 10.f, 25.f, 5.f);
	/*EnemyComponent* ec3 = */registry.AddComponent<EnemyComponent>(skeleton2, 2);

	/*ControllerComponent* cc = */registry.AddComponent<ControllerComponent>(player);

	ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	/*PointOfInterestComponent* poic = */registry.AddComponent<PointOfInterestComponent>(player, poic);
	/*PointOfInterestComponent* dogPoi = */registry.AddComponent<PointOfInterestComponent>(dog);
	/*PointOfInterestComponent* skelPoi = */registry.AddComponent<PointOfInterestComponent>(skeleton);
	/*PointOfInterestComponent* skelPoi2 = */registry.AddComponent<PointOfInterestComponent>(skeleton2);

	//ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
	////particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/FullHealth.png"), UIText(L""));
	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, 1.0f, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/EmptyHealth.png"), UIText(L""));

	std::string valueAsString = "1";
	std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());
	UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, 1.0f, DirectX::XMFLOAT2(0.9f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), UIImage("ExMenu/CheckboxBase.png"), UIText(valueAsWString));


	/*HellhoundBehaviour* hellhoundBehevCo = */registry.AddComponent<HellhoundBehaviour>(dog);
	/*SkeletonBehaviour* skeletonBehevCo = */registry.AddComponent<SkeletonBehaviour>(skeleton);
	/*SkeletonBehaviour* skeletonBehevCo2 = */registry.AddComponent<SkeletonBehaviour>(skeleton2);


	RenderGeometryIndependentCollision(stage);
	//Finally set the collision boxes
	SetupPlayerCollisionBox(player, 1.0f);

	SetupEnemyCollisionBox(skeleton, 0.9f);
	SetupEnemyCollisionBox(skeleton2, 0.9f);
	SetupEnemyCollisionBox(dog, 1.0f);

	
}
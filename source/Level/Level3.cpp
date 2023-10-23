#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "Model.h"
#include "UIComponents.h"

void LoadLevel3()
{
	//Doggo
	EntityID gameLevel = registry.CreateEntity();
	//EntityID dog = registry.CreateEntity();
	EntityID stage = registry.CreateEntity();
	EntityID player = registry.CreateEntity();
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();


	//**************************************************
	EntityID tempBoss = registry.CreateEntity();
	SetupEnemy(tempBoss, enemyType::tempBoss, 10.f, 0.f, 2.f, 50.f, 400.f, 10.f, 20.f, 3.f, 4, 1.f, 0.f, 1.f, 4.f, 4.f, 4.f);

	registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("HellhoundDummy_PH.mdl"));
	registry.AddComponent<ModelSkeletonComponent>(player, LoadModel("PlayerPlaceholder.mdl"));
	registry.AddComponent<AnimationComponent>(player, AnimationComponent());



	TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
	stc->scaleX = 60.0f;
	stc->scaleZ = 30.0f;
	// Player (Default)
	registry.AddComponent<TransformComponent>(player);



	registry.AddComponent<StatComponent>(player, 125.f, 20.0f, 50.f, 5.0f); //Hp, MoveSpeed, Damage, AttackSpeed
	registry.AddComponent<PlayerComponent>(player);



	registry.AddComponent<ControllerComponent>(player);

	ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;
	registry.AddComponent<PointOfInterestComponent>(player, poic);

	//ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
	////particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.8f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->text.Setup(L"");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(player, DirectX::XMFLOAT2(-0.8f, 0.6f), DirectX::XMFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup(L"");

	//Thing in the top right corner showing what level we're on
	//std::string valueAsString = "2";
	//std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());
	UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(gameLevel, DirectX::XMFLOAT2(0.9f, 0.9f), DirectX::XMFLOAT2(1.0f, 1.0f), 2);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup(L"");



	RenderGeometryIndependentCollision(stage);
	//Finally set the collision boxes
	SetupPlayerCollisionBox(player, 1.0f);


	MouseComponentAddComponent(player);

}
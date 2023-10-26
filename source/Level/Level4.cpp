#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "Particles.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "Model.h"
#include "UIComponents.h"

void LoadLevel4()
{
	//EntityID dog = registry.CreateEntity();
	EntityID stage = registry.CreateEntity();
	EntityID playerUi = registry.CreateEntity();
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID particle = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//**************************************************
	EntityID tempBoss = registry.CreateEntity();
	SetupEnemy(EnemyType::tempBoss, 10.f, 0.f, 2.f, 50.f, 400.f, 10.f, 20.f, 0.5f, 4, 4.f, 4.f, 4.f);

	registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");


	TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
	stc->scaleX = 1.0f;
	stc->scaleZ = 1.0f;


	ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 10.f, 5.f, 2.f, 1.f, 1.f, 1.f, SMOKE);
	PointOfInterestComponent poic;
	poic.weight = 10.0f;

	//ParticleComponent* particComp = registry.AddComponent<ParticleComponent>(particle, renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);
	////particComp->Setup(renderStates, Particles::RenderSlot, 5.f, 5.f, 2.f, 0.f, 0.f, 0.f, SMOKE);

	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	pcUiHpC->backgroundImage.Setup("ExMenu/EmptyHealth.png");
	pcUiHpC->healthImage.Setup("ExMenu/FullHealth.png");
	pcUiHpC->text.Setup("");

	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));
	pcUiSC->image.Setup("ExMenu/EmptyHealth.png");
	pcUiSC->text.Setup("");

	//Thing in the top right corner showing what level we're on
	UIGameLevelComponent* gameLevelUIc = registry.AddComponent<UIGameLevelComponent>(stage, DSFLOAT2(0.9f, 0.9f), DSFLOAT2(1.0f, 1.0f), 4);
	gameLevelUIc->image.Setup("ExMenu/CheckboxBase.png");
	gameLevelUIc->text.Setup("");

	RenderGeometryIndependentCollision(stage);

}
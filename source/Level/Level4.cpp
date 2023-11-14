#include "Levels\LevelLoader.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "Components.h"
#include "CollisionFunctions.h"
#include "Levels\LevelHelper.h"
#include "Model.h"
#include "UIComponents.h"
#include "States\StateManager.h"
#include "EventFunctions.h"

void LoadLevel4()
{
	//EntityID dog = registry.CreateEntity();
	EntityID stage = registry.CreateEntity();
	//EntityID skeleton = registry.CreateEntity();
	//EntityID skeleton2 = registry.CreateEntity();
	EntityID portal = registry.CreateEntity();

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//**************************************************
	EntityID tempBoss = SetupEnemy(EnemyType::tempBoss, 10.f, 0.f, 2.f, 50.f, 400.f, 10.f, 20.f, 0.5f, 4, 4.f, 4.f, 4.f);
	Stage4IntroScene(tempBoss, 0);
	registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");


	TransformComponent* stc = registry.AddComponent<TransformComponent>(stage);
	stc->scaleX = 1.0f;
	stc->scaleZ = 1.0f;


	PointOfInterestComponent poic;
	poic.weight = 10.0f;



	RenderGeometryIndependentCollision(stage);


	EntityID mouse = registry.CreateEntity();
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	SetDirectionLight(1.1f, 1.0f, .9f, -1.6f, -2.0f, 1.0f);

	stateManager.stage = stage;
	SetInPlay(true);
}
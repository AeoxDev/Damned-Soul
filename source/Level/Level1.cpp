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
#include "UIButtonFunctions.h"

void LoadLevel1()
{
	float redMult = .75f;
	float greenMult = .75f;
	float blueMult = .75f;
	EntityID stage = SetUpStage(redMult, greenMult, blueMult, 0, 0, 0, 0.9); //registry.CreateEntity();

	EntityID mouse = registry.CreateEntity();
	EntityID timeEntity = registry.CreateEntity(ENT_PERSIST_LEVEL);

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	CreatePlayer(0.0f, 0.0f, 0.0f, 80.0f, 100.0f, 20.0f,		10.0f,	1.0f,		 1,			0.0f, 0.0, -1.0f);
	//			 posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	//EntityID cutsceneEnemy = SetupEnemy(EnemyType::skeleton, -45.f, 0.f, -20.f);
	//SetupEnemy(EnemyType::skeleton, 40.f, 0.f, -35.f);
	//SetupEnemy(EnemyType::skeleton, -30.f, 0.f, 45.f);
	//SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 45.f);
	//SetupEnemy(EnemyType::skeleton, -40.f, 0.f, 35.f);
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::lucifer, 0.f, 0.f, 0.f, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 6969.f, 2.f, 2.f, 2.f);
	//registry.AddComponent<ParticleComponent>(stage, 5.0f, 10.f, 0.5f, 0.0f, 0.0f, 1.0f, SMOKE);
	//5 souls total
	
	/*char ctexture[] = "1-1C.png";
	char emptyTexture[] = "";
	AddStaticHazardTexture(stage, ctexture, emptyTexture, emptyTexture);*/

	RenderGeometryIndependentCollision(stage);
	
	//Finally set the collision boxes

	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;

	//CreatePointLight(player, 1.0f, 0.1f, 0.1f, 0.0f, 1.0f, 0.0f, 100.0f, 10.0f);
	SetDirectionLight(1.1f, 1.0f, .9f, -1.6f, -2.0f, 1.0f);
	CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	//CreatePointLight(lightholder, 0.8f, 0.0f, 0.0f, 70.0f, 20.0f, 35.0f, 140.0f, 10.0f);
	CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	UIComponent* uiElement = registry.AddComponent<UIComponent>(timeEntity);
	uiElement->Setup("TempShopTitle", "Time: 0", DSFLOAT2(0.8f, 0.8f));
	uiElement->m_BaseImage.baseUI.SetVisibility(false);

	UIRunTimeComponent* runtime = registry.AddComponent<UIRunTimeComponent>(timeEntity);


	stateManager.stage = stage;

	SetInPlay(true);
	//AddTimedEventComponentStart(cutsceneEnemy, 0.0f, Stage1IntroScene, 0, 1);
	Stage1IntroScene(cutsceneEnemy, 0);
}
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

#include "SDLHandler.h"

void SetScoreboardUI(EntityID stage)
{
	//Scoreboard UI
	UIComponent* uiElement = registry.AddComponent<UIComponent>(stage);
	uiElement->Setup("TempShopWindow3", "Run Completed!", DSFLOAT2(0.0f, 0.0f), DSFLOAT2(1.5f, 1.0f));
	uiElement->m_BaseText.baseUI.SetPosition(DSFLOAT2(0.0f, 0.6f));

	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stage);
	OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(stage);

	uiElement->AddImage("ExMenu/ButtonBackground", DSFLOAT2(-0.2f, -0.6f), DSFLOAT2(0.5f, 0.6f));
	uiElement->AddText("\nNew Run", uiElement->m_Images[0].baseUI.GetBounds(), DSFLOAT2(-0.2f, -0.6f));

	onClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), 1, UIFunc::MainMenu_Start);
	onHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunc::HoverImage);

	uiElement->AddImage("ExMenu/ButtonBackground", DSFLOAT2(0.2f, -0.6f), DSFLOAT2(0.5f, 0.6f));
	uiElement->AddText("\nMain Menu", uiElement->m_Images[1].baseUI.GetBounds(), DSFLOAT2(0.2f, -0.6f));

	onClick->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), 1, UIFunc::Game_MainMenu);
	onHover->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunc::HoverImage);

	DSFLOAT2 offsetUICoords = { abs(uiElement->m_BaseImage.baseUI.GetPixelCoords().x + 32.0f) ,
						   abs(uiElement->m_BaseImage.baseUI.GetPixelCoords().y + 32.0f) };

	DSFLOAT2 uiPixelCoords = { (offsetUICoords.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
						-1 * ((offsetUICoords.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

	DSFLOAT2 diffPos(uiPixelCoords.x + 1.1f, uiPixelCoords.y - 0.3f);

	uiElement->AddText("Difficulty", uiElement->m_Images[1].baseUI.GetBounds(), DSFLOAT2(diffPos.x, diffPos.y));
	uiElement->AddImage("Slider1", DSFLOAT2(diffPos.x, diffPos.y - 0.05f));
	uiElement->AddImage("Slider2", DSFLOAT2(diffPos.x, diffPos.y - 0.15f));

	const int amount = 11;
	const char const texts[amount][32] =
	{
		"Time: ", //score

		"Total Souls: ",
		"Leftover Souls: ",  //score
		"Spent Souls: ",  //-score

		"Damage Done: ", //score

		"Strongest Hit Dealt: ", //score

		"Damage Taken: ", //score

		"Strongest Hit Taken: ",

		"Healing Done: ", //score

		"Strongest Heal Done: ", //score

		"Score: "
	};

	for (int i = 0; i < amount; i++)
		uiElement->AddText(texts[i], DSBOUNDS(0.0f, 0.0f, 0.0f, 0.0f), DSFLOAT2(uiPixelCoords.x + 0.2f, uiPixelCoords.y - 0.2f - (0.1f * i)));

	UIGameScoreboardComponent* scoreBoard = registry.AddComponent<UIGameScoreboardComponent>(stage);

	uiElement->SetAllVisability(false);
}

void LoadLevel1()
{
	
	EntityID stage = registry.CreateEntity();
	EntityID mouse = registry.CreateEntity();
	EntityID timeEntity = registry.CreateEntity(ENT_PERSIST_LEVEL);

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();
	CreatePlayer(0.0f, 0.0f, 0.0f, 80.0f, 100.0f, 20.0f,	10.0f,		1.0f,		 1,		 0.0f,	 0.0,	-1.0f);
	//			 posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth, scaleX, ScaleY, ScaleZ
	EntityID cutsceneEnemy = SetupEnemy(EnemyType::skeleton, -45.f, 0.f, -20.f);
	SetupEnemy(EnemyType::skeleton, 40.f, 0.f, -35.f);
	SetupEnemy(EnemyType::skeleton, -30.f, 0.f, 45.f);
	SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 45.f);
	SetupEnemy(EnemyType::skeleton, -40.f, 0.f, 35.f);
	SetupEnemy(EnemyType::skeleton, -20.f, 0.f, 20.f);

	//registry.AddComponent<ParticleComponent>(stage, 5.0f, 10.f, 0.5f, 0.0f, 0.0f, 1.0f, SMOKE);
	//5 souls total


	ModelBonelessComponent* stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	stageModel->colorMultiplicativeRed = 0.75f;
	stageModel->colorMultiplicativeGreen = 0.75f;
	stageModel->colorMultiplicativeBlue = 0.75f;
	stageModel->gammaCorrection = 0.9f;
	//stageModel->colorAdditiveRed = 0.1f;
	
	// Stage (Default)
	TransformComponent *stageTransform = registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");	
	
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

	UIGameTimeComponent* runtime = registry.AddComponent<UIGameTimeComponent>(timeEntity);

	SetScoreboardUI(stage);

	stateManager.stage = stage;

	SetInPlay(true);
	//AddTimedEventComponentStart(cutsceneEnemy, 0.0f, Stage1IntroScene, 0, 1);
	Stage1IntroScene(cutsceneEnemy, 0);
}
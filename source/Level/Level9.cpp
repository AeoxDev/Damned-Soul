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

//void SetScoreboardUI(EntityID stage)
//{
//	//Scoreboard UI
//	UIComponent* uiElement = registry.AddComponent<UIComponent>(stage);
//	uiElement->Setup("TempShopWindow3", "Run Completed!", DSFLOAT2(0.0f, 0.2f));
//
//	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(stage);
//	OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(stage);
//
//	uiElement->AddImage("ExMenu/ButtonBackground", DSFLOAT2(-0.2f, -0.6f));
//	uiElement->AddText("New Run", uiElement->m_Images[0].baseUI.GetBounds(), DSFLOAT2(-0.2f, -0.6f));
//
//	onClick->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), 1, UIFunc::MainMenu_Start);
//	onHover->Setup(uiElement->m_Images[0].baseUI.GetPixelCoords(), uiElement->m_Images[0].baseUI.GetBounds(), UIFunc::HoverImage);
//
//	uiElement->AddImage("ExMenu/ButtonBackground", DSFLOAT2(0.2f, -0.6f));
//	uiElement->AddText("Main Menu", uiElement->m_Images[1].baseUI.GetBounds(), DSFLOAT2(0.2f, -0.6f));
//
//	onClick->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), 1, UIFunc::Game_MainMenu);
//	onHover->Setup(uiElement->m_Images[1].baseUI.GetPixelCoords(), uiElement->m_Images[1].baseUI.GetBounds(), UIFunc::HoverImage);
//
//	const int amount = 19;
//
//	const char const texts[amount][32] =
//	{
//		"Time: ",
//		"Best Time: ",
//
//		"Souls: ",
//		"Leftover Souls: ",  //score
//		"Spent Souls: ",  //-score
//		"Total Souls: ",
//
//		"Damage Done: ", //score
//		"Total Damage Done: ",
//
//		"Highest Damage Hit: ", //score
//		"Best Highest Damage Hit: ",
//
//		"Damage Taken: ", //score
//		"Total Damage Taken: ",
//
//		"Highest Damage Taken: ", //score
//		"Best Highest Damage Taken: ",
//
//		"Healing Done: ", //score
//		"Total Healing Done: ",
//
//		"Highest Healing Done: ", //score
//		"Best Highest Healing Done: ",
//
//		"Score: "
//	};
//
//	const DSFLOAT2 const positions[amount] =
//	{
//		{ 0.4f, 0.3f },
//		{ 0.0f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f },
//		{ -0.4f, 0.3f }
//	};
//
//	for (int i = 0; i < amount; i++)
//	{
//		uiElement->AddText(texts[i], DSBOUNDS(0.0f, 0.0f, 0.0f, 0.0f), positions[i]);
//	}
//
//	UIGameScoreboardComponent* scoreBoard = registry.AddComponent<UIGameScoreboardComponent>(stage);
//
//	uiElement->SetAllVisability(false);
//}

void LoadLevel9()
{
	//This is the eye stage. Lots of acid in a grey environment.
	EntityID stage = registry.CreateEntity();

	EntityID mouse = registry.CreateEntity();

	//StageLights
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	//posX, posY, posZ, mass, health, moveSpeed, damage, attackSpeed, soulWorth
	SetupEnemy(EnemyType::skeleton, -25.f, 0.f, 50.f);
	SetupEnemy(EnemyType::skeleton, 50.f, 0.f, -45.f);
	SetupEnemy(EnemyType::eye, -20.f, 0.f, 25.f);
	SetupEnemy(EnemyType::eye, 30.f, 0.f, -25.f);
	SetupEnemy(EnemyType::eye, -50.f, 0.f, 45.f);
	SetupEnemy(EnemyType::eye, 35.f, 0.f, 25.f);
	SetupEnemy(EnemyType::eye, 15.f, 0.f, -45.f);
	SetupEnemy(EnemyType::eye, 35.f, 1.f, 45.f);
	SetupEnemy(EnemyType::eye, -25.f, 1.f, -35.f);
	SetupEnemy(EnemyType::eye, -50.f, 1.f, 25.f);
	SetupEnemy(EnemyType::eye, -40.f, 1.f, 25.f);
	SetupEnemy(EnemyType::eye, -55.f, 1.f, -35.f);
	SetupEnemy(EnemyType::hellhound, -32.f, 1.f, 28.f);
	SetupEnemy(EnemyType::hellhound, 13.f, 1.f, -12.f);

	EntityID cutsceneEnemy = SetupEnemy(EnemyType::eye, -40.f, 0.f, -45.f);
	Stage3IntroScene(cutsceneEnemy, 0);

	//Stage
	float redAdd = 0.0f;
	float greenAdd = 0.0f;
	float blueAdd = 0.1f;
	float redMult = 1.0f;
	float greenMult = 1.0f;
	float blueMult = 1.1f;

	ModelBonelessComponent* stageModel = registry.AddComponent<ModelBonelessComponent>(stage, LoadModel("PlaceholderScene.mdl"));
	stageModel->colorMultiplicativeRed = redMult;
	stageModel->colorMultiplicativeGreen = greenMult;
	stageModel->colorMultiplicativeBlue = blueMult;
	stageModel->colorAdditiveBlue = blueAdd;
	stageModel->colorAdditiveRed = redAdd;

	registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");

	//Player
	ReloadPlayerNonGlobals();//Bug fix if player dashes into portal

	//SetScoreboardUI(stage);

	//Mouse
	registry.AddComponent<TransformComponent>(mouse);
	PointOfInterestComponent* mousePointOfInterset = registry.AddComponent<PointOfInterestComponent>(mouse);
	mousePointOfInterset->mode = POI_MOUSE;


	//Light
	float redLight = 0.05f;
	float greenLight = 0.05f;
	float blueLight = 0.25f;

	SetDirectionLight(1.0f, 1.1f, 1.0f, -1.6f, -2.0f, 1.0f);
	CreatePointLight(stage, 0.4f, 0.6f, 0.15f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
	CreatePointLight(lightholder, redLight, greenLight, blueLight, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderTwo, redLight, greenLight, blueLight, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderThree, redLight, greenLight, blueLight, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
	CreatePointLight(lightholderForth, redLight, greenLight, blueLight, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

	//Add static hazards on the where player does not spawn
	int nrHazards = 8;
	for (size_t i = 0; i < nrHazards; i++)
	{
		bool succeded = false;
		while (!succeded)
		{
			float randX = (float)(rand() % 100) - 50.0f;
			float randZ = (float)(rand() % 100) - 50.0f;
			if (randX * randX + randZ * randZ > 80)
			{
				float randScaleX = 5.0f + (float)((rand() % 100) * 0.1f);
				float randScaleZ = 5.0f + (float)((rand() % 100) * 0.1f);
				EntityID hazard = registry.CreateEntity();
				ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
				hazardModel->colorAdditiveRed = 0.0f;
				hazardModel->colorAdditiveGreen = 0.5f;
				hazardModel->colorAdditiveBlue = 0.0f;
				hazardModel->colorMultiplicativeRed = 0.2f;
				hazardModel->colorMultiplicativeGreen = 1.2f;
				hazardModel->colorMultiplicativeBlue = 0.2f;
				hazardModel->gammaCorrection = 1.5f;
				hazardModel->castShadow = false;
				TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
				hazardTransform->positionX = randX;
				hazardTransform->positionY = 0.5f;
				hazardTransform->positionZ = randZ;
				hazardTransform->scaleX = randScaleX;
				hazardTransform->scaleY = 1.0f;
				hazardTransform->scaleZ = randScaleZ;
				hazardTransform->facingX = cosf((float)rand());
				hazardTransform->facingZ = sinf((float)rand());
				AddStaticHazard(hazard, HAZARD_ACID);

				succeded = true;
			}
		}
	}
	nrHazards = 1;//Big ice sheet
	for (size_t i = 0; i < nrHazards; i++)
	{
		bool succeded = false;
		float randX = (float)(rand() % 32) - 16.0f;
		float randZ = (float)(rand() % 32) - 16.0f;
		float randScaleX = 64.0f + (float)((rand() % 100) * 0.1f);
		float randScaleZ = 64.0f + (float)((rand() % 100) * 0.1f);
		EntityID hazard = registry.CreateEntity();
		ModelBonelessComponent* hazardModel = registry.AddComponent<ModelBonelessComponent>(hazard, LoadModel("LavaPlaceholder.mdl"));
		hazardModel->colorAdditiveRed = 0.1f;
		hazardModel->colorAdditiveGreen = 0.1f;
		hazardModel->colorAdditiveBlue = 0.5f;
		hazardModel->colorMultiplicativeRed = 0.5f;
		hazardModel->colorMultiplicativeGreen = 0.5f;
		hazardModel->colorMultiplicativeBlue = 1.5f;
		hazardModel->gammaCorrection = 1.5f;
		hazardModel->castShadow = false;
		TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
		hazardTransform->positionX = randX;
		hazardTransform->positionY = 0.4f;
		hazardTransform->positionZ = randZ;
		hazardTransform->scaleX = randScaleX;
		hazardTransform->scaleY = 1.0f;
		hazardTransform->scaleZ = randScaleZ;
		hazardTransform->facingX = cosf((float)rand());
		hazardTransform->facingZ = sinf((float)rand());
		AddStaticHazard(hazard, HAZARD_ICE);

		succeded = true;
	}
	RenderGeometryIndependentCollision(stage);

	stateManager.stage = stage;
	SetInPlay(true);
}
#include "States\Menu.h"
#include "UI/UIRenderer.h"
#include "Hitbox.h"
#include "States\StateManager.h"
#include "Input.h"
#include "Registry.h"
#include "Components.h"
#include "Camera.h"
#include "States\CleanupMacros.h"
#include "UI/UIButtonFunctions.h"
#include "Level.h"
#include "Model.h"
#include "UIComponents.h"
#include "Relics/RelicFunctions.h"
#include "Levels\LevelHelper.h"
#include "SDLHandler.h"
#include "EventFunctions.h"
#include "DeltaTime.h"

void Menu::Setup()
{
	// Clear relics when entering the main menu
	Relics::ResetRelics();
	SetInMainMenu(true);
	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
	Camera::ResetCamera();

	ResetRunTime();

	//Setup stage to rotate around
	EntityID stage = registry.CreateEntity();
	
	// Stage Transform
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	ProximityHitboxComponent* phc = registry.AddComponent<ProximityHitboxComponent>(stage);
	phc->Load("default");
	// Stage POI
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);
	stageP->mode = POI_FORCE;
	stageP->height = CAMERA_OFFSET_Y * -0.75f;
	stageP->rotationY = 0.0f;
	stageP->rotationRadius = -0.84f * CAMERA_OFFSET_Z;
	stageP->rotationAccel = 0.08f;
	SetDirectionLight(1.1f, 1.0f, .9f, -1.6f, -2.0f, 1.0f);

	/*char ctexture[] = "3-1C.png";
	char ltexture[] = "3-1L.png";
	char emptyTexture[] = "";
	AddStaticHazardTexture(stage, ctexture, ltexture, emptyTexture);*/
	stateManager.player = registry.CreateEntity();
	registry.AddComponent<PlayerComponent>(stateManager.player);

	stateManager.activeLevel = 0;
	AddTimedEventComponentStart(stage, 2.0f, LoopSpawnMainMenuEnemy, skeleton, 1);
	EntityID enemy = SetupEnemy(EnemyType::skeleton, 0.0f, 0.f, 0.0f);
	SetHitboxIsPlayer(enemy, 1, true);
	StatComponent* stats = registry.GetComponent<StatComponent>(enemy);

	//Randomize enemies on screen max 6 of each'
	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 8 == 0)//Dog, rare
			RandomPlayerEnemy(hellhound);
	}

	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 16 == 0)//Eye, very rare
			RandomPlayerEnemy(eye);
	}

	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 2 == 0)//Skeleton, common
			RandomPlayerEnemy(skeleton);
	}
	if (rand() % 4096 == 0)//Boss, Pokemon Shiny rarity :)
		RandomPlayerEnemy(tempBoss);
	int random = rand() % 4;//Level 1, 2, 3
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	// Stage Model
	ModelBonelessComponent* stageModel = registry.AddComponent<ModelBonelessComponent>(stage);
	stageModel->model = LoadModel("PlaceholderScene.mdl");
	int nrHazards = 8;
	switch (random)
	{
	case 0: //level 1
		CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		//CreatePointLight(lightholder, 0.8f, 0.0f, 0.0f, 70.0f, 20.0f, 35.0f, 140.0f, 10.0f);
		CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		stageModel->colorMultiplicativeRed = 0.75f;
		stageModel->colorMultiplicativeGreen = 0.75f;
		stageModel->colorMultiplicativeBlue = 0.75f;
		break;
	case 1: //Lava
		CreatePointLight(stage, 0.6f, 0.6f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		CreatePointLight(lightholder, 0.35f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.35f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.35f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.35f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

		stageModel->colorMultiplicativeRed = 1.4f;
		stageModel->colorMultiplicativeGreen = 1.2f;
		stageModel->colorMultiplicativeBlue = 0.8f;
		stageModel->colorAdditiveRed = 0.1f;
		
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
					hazardModel->gammaCorrection = 1.5f;
					TransformComponent* hazardTransform = registry.AddComponent<TransformComponent>(hazard);
					hazardTransform->positionX = randX;
					hazardTransform->positionY = 0.1f;
					hazardTransform->positionZ = randZ;
					hazardTransform->scaleX = randScaleX;
					hazardTransform->scaleY = 0.1f;
					hazardTransform->scaleZ = randScaleZ;
					hazardTransform->facingX = cosf((float)rand());
					hazardTransform->facingZ = sinf((float)rand());
					AddStaticHazard(hazard, HAZARD_LAVA);

					succeded = true;
				}
			}
		}
		break;
	case 2://Ice
		CreatePointLight(stage, 0.4f, 0.5f, 0.2f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		CreatePointLight(lightholder, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.10f, 0.0f, 0.3f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.10f, 0.0f, 0.3f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		stageModel->colorMultiplicativeRed = 1.2f;
		stageModel->colorMultiplicativeGreen = 1.0f;
		stageModel->colorMultiplicativeBlue = 1.4f;
		stageModel->colorAdditiveBlue = 0.1f;
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
		break;
	default:
		CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		//CreatePointLight(lightholder, 0.8f, 0.0f, 0.0f, 70.0f, 20.0f, 35.0f, 140.0f, 10.0f);
		CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		break;
	}
	

	
	RenderGeometryIndependentCollision(stage);
	stateManager.stage = stage;
	Camera::SetCutsceneMode(false);
	AddTimedEventComponentStart(stage, (8.0f + (float)(rand() % 32))* (float)(rand() % 2), MainMenuIntroCutscene);
}

void Menu::Input()
{
	
}

void Menu::SetupButtons()
{
	const int buttons = 4;

	const char const texts[buttons][32] =
	{
		"\nStart",
		"\nCredits",
		"\nSettings",
		"\nQuit"
	};
	
	const DSFLOAT2 const positions[buttons] =
	{
		{ -0.81f, -0.02f },
		{ -0.81f, -0.28f },
		{ -0.81f,  -0.54f },
		{ -0.81f, -0.8f }
	};

	const DSFLOAT2 const scales[buttons] =
	{
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f }
	};

	void(* const functions[buttons])(void*, int) =
	{
		UIFunc::MainMenu_Start,
		UIFunc::MainMenu_Credits,
		UIFunc::MainMenu_Settings,
		UIFunc::MainMenu_Quit
	};

	for (int i = 0; i < buttons; i++)
	{
		auto button = registry.CreateEntity();
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i]);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}
}

void Menu::SetupImages()
{
	// Damned Soul Main Menu Title
	auto title = registry.CreateEntity();
	UIComponent* uiElement = registry.AddComponent<UIComponent>(title);
	uiElement->Setup("ExMenu/DamnedTitle3", "", { 0.0f, 0.20f });
}

void Menu::SetupText()
{

}

void Menu::Unload()
{
	SetInMainMenu(false);
	UnloadEntities();
	ClearUI();
}
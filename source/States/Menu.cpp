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
	gameSpeed = 1.0f;
	// Clear relics when entering the main menu
	Relics::ResetRelics();
	SetInMainMenu(true);
	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
	Camera::ResetCamera();

	ResetRunTime();
	int random = rand() % 5;//Level 1, 2, 3, 4
	//Setup stage to rotate around
	StageSetupVariables stageVars;
	stageVars.stageNr = random;
	stageVars.scaleX = 1.0f;
	stageVars.scaleY = 1.0f;
	stageVars.scaleZ = 1.0f;
	stageVars.offsetY = -.1f;
	//stageVars.offsetX = 16.f;
	EntityID stage = SetUpStage(stageVars); //registry.CreateEntity();


	SetDirectionLight(1.0f, 0.8f, 0.6f, -1.6f, -3.0f, 1.0f);

	/*char ctexture[] = "3-1C.png";
	char ltexture[] = "3-1L.png";
	char emptyTexture[] = "";
	AddStaticHazardTexture(stage, ctexture, ltexture, emptyTexture);*/
	stateManager.player = registry.CreateEntity();
	registry.AddComponent<PlayerComponent>(stateManager.player);

	stateManager.activeLevel = 0;
	AddTimedEventComponentStart(stage, 2.0f, LoopSpawnMainMenuEnemy, skeleton, 1);
	/*EntityID enemy = SetupEnemy(EnemyType::skeleton, 0.0f, 0.f, 0.0f);
	SetHitboxIsPlayer(enemy, 1, true);
	StatComponent* stats = registry.GetComponent<StatComponent>(enemy);*/

	//Randomize enemies on screen max 12 of each'
	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 16 == 0)//Dog, rare
			RandomPlayerEnemy(hellhound);
	}
	//Randomize enemies on screen max 6 of each'
	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 8 == 0)//imp, uncommon
			RandomPlayerEnemy(imp);
	}

	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 32 == 0)//Eye, very rare
			RandomPlayerEnemy(eye);
	}

	for (size_t i = 0; i < 12; i++)
	{
		if (rand() % 2 == 0)//Skeleton, common
			RandomPlayerEnemy(skeleton);
	}
	if (rand() % 4096 == 0)//Boss, Pokemon Shiny rarity :)
		RandomPlayerEnemy(tempBoss);
	
	EntityID lightholder = registry.CreateEntity();
	EntityID lightholderTwo = registry.CreateEntity();
	EntityID lightholderThree = registry.CreateEntity();
	EntityID lightholderForth = registry.CreateEntity();

	// Stage Model
	ModelBonelessComponent* stageModel = registry.GetComponent<ModelBonelessComponent>(stage);
	int nrHazards = 8;
	// Stage POI
	EntityID poi = registry.CreateEntity();
	TransformComponent* poiPoint = registry.AddComponent<TransformComponent>(poi);
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(poi);
	stageP->mode = POI_FORCE;
	stageP->rotationY = 0.0f;
	stageP->rotationAccel = 0.08f;
	stageP->rotationRadius = -0.8f * CAMERA_OFFSET_Z;
	stageP->height = -0.2f * CAMERA_OFFSET_Y;
	switch (stageVars.stageNr)
	{
	case 0: //level 1
	case 1: //Level 1
		CreatePointLight(stage, 0.5f, 0.5f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		//CreatePointLight(lightholder, 0.8f, 0.0f, 0.0f, 70.0f, 20.0f, 35.0f, 140.0f, 10.0f);
		CreatePointLight(lightholder, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.30f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.30f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.30f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		stageP->rotationRadius = -0.8f * CAMERA_OFFSET_Z;
		stageP->height = -0.2f * CAMERA_OFFSET_Y;
		poiPoint->positionX = -104.0f;
		poiPoint->positionZ = -22.0f;
		break;
	case 2://level 2
		CreatePointLight(stage, 0.4f, 0.5f, 0.2f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		CreatePointLight(lightholder, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.10f, 0.0f, 0.3f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.10f, 0.0f, 0.3f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		stageP->rotationRadius = -0.8f * CAMERA_OFFSET_Z;
		stageP->height = -0.2f * CAMERA_OFFSET_Y;
		poiPoint->positionX = -115.0f;
		poiPoint->positionZ = 94.0f;
		break;
	case 3://level 3
		CreatePointLight(stage, 0.4f, 0.5f, 0.2f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		CreatePointLight(lightholder, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.10f, 0.0f, 0.3f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.10f, 0.0f, 0.3f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		stageP->rotationRadius = -0.8f * CAMERA_OFFSET_Z;
		stageP->height = -0.2f * CAMERA_OFFSET_Y;
		poiPoint->positionX = -142.0f;
		poiPoint->positionZ = 72.0f;
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

	const char texts[buttons][32] =
	{
		"Start",
		"Credits",
		"Settings",
		"Quit"
	};
	
	const DSFLOAT2 positions[buttons] =
	{
		{ -0.81f, -0.02f },
		{ -0.81f, -0.28f },
		{ -0.81f,  -0.54f },
		{ -0.81f, -0.8f }
	};

	const DSFLOAT2 scales[buttons] =
	{
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f },
		{ 0.7f, 0.6f }
	};

	void(* const functions[buttons])(void*, int) =
	{
		UIFunctions::MainMenu::Start,
		UIFunctions::MainMenu::SetCredits,
		UIFunctions::MainMenu::SetSettings,
		UIFunctions::MainMenu::Quit
	};

	for (int i = 0; i < buttons; i++)
	{
		auto button = registry.CreateEntity();
		OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
		OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
		UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

		uiElement->Setup("Exmenu/ButtonBackground", texts[i], positions[i], scales[i], 30.0f);

		onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, functions[i]);
		onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::Image);

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
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
	m_active = true;
	
	// Clear relics when entering the main menu
	Relics::ResetRelics();

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
	Camera::ResetCamera();
	
	stateManager.player = registry.CreateEntity(ENT_PERSIST_LEVEL);

	//Temp stuff for ui to not crash because saving between levels is not fully implemented
	EntityID playerUi = registry.CreateEntity();
	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));

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


	stateManager.activeLevel = 0;
	AddTimedEventComponentStart(stage, 2.0f, LoopSpawnMainMenuEnemy, skeleton, 1);
	EntityID enemy = SetupEnemy(EnemyType::skeleton, 0.0f, 0.f, 0.0f);
	SetHitboxIsPlayer(enemy, 1, true);
	registry.AddComponent<PlayerComponent>(stateManager.player);
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
	case 1: //level 2
		CreatePointLight(stage, 0.6f, 0.6f, 0.0f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		CreatePointLight(lightholder, 0.35f, 0.0f, 0.0f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.35f, 0.0f, 0.0f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.35f, 0.0f, 0.0f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.35f, 0.0f, 0.0f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);

		stageModel->colorMultiplicativeRed = 1.4f;
		stageModel->colorMultiplicativeGreen = 1.2f;
		stageModel->colorMultiplicativeBlue = 0.8f;
		stageModel->colorAdditiveRed = 0.1f;
		break;
	case 2:
		CreatePointLight(stage, 0.4f, 0.5f, 0.2f, -90.0f, 20.0f, -35.0f, 90.0f, 10.0f);// needs to be removed end of level
		CreatePointLight(lightholder, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, 40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderTwo, 0.10f, 0.0f, 0.3f, 70.0f, 20.0f, -40.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderThree, 0.10f, 0.0f, 0.3f, 0.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		CreatePointLight(lightholderForth, 0.10f, 0.0f, 0.3f, -70.0f, 20.0f, -80.0f, 140.0f, 10.0f);
		stageModel->colorMultiplicativeRed = 1.2f;
		stageModel->colorMultiplicativeGreen = 1.0f;
		stageModel->colorMultiplicativeBlue = 1.4f;
		stageModel->colorAdditiveBlue = 0.1f;
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
	

	const int nrHazards = 8;
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
	RenderGeometryIndependentCollision(stage);
	stateManager.stage = stage;
}

void Menu::Input()
{
	
}

void Menu::SetupButtons()
{
	//Start Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", "Start", UIFunc::LoadNextLevel, { -0.81f, -0.28f }, {0.7f, 0.6f} );
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}

	//Settings Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", "Settings", UIFunc::MainMenu_Settings, { -0.81f,  -0.54f }, {0.7f, 0.6f} );
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}

	//Exit Button
	{
		auto button = registry.CreateEntity();
		UIButton* comp = registry.AddComponent<UIButton>(button);
		comp->Setup("Exmenu/ButtonBackground.png", "Exmenu/ButtonBackgroundHover.png", "Quit", UIFunc::MainMenu_Quit, { -0.81f, -0.8f }, {0.7f, 0.6f} );
		SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
		buttonSound->Load(MENU);
	}
}

void Menu::SetupImages()
{
	// Damned Soul Main Menu Title
	auto title = registry.CreateEntity();
	auto tc = registry.AddComponent<UIImage>(title);
	tc->Setup("ExMenu/DamnedTitle3.png", { 0.0f, 0.20f }, { 1.0f, 1.0f});
}

void Menu::SetupText()
{

}

void Menu::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	UnloadEntities();

	ClearUI();
}
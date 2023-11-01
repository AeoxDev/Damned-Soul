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

#include "SDLHandler.h"

void Menu::Setup()//Load
{
	m_active = true;
	// Clear relics when entering the main menu
	Relics::ResetRelics();

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();
	Camera::ResetCamera();
	

	//Temp stuff for ui to not crash because saving between levels is not fully implemented
	EntityID playerUi = registry.CreateEntity();
	UIHealthComponent* pcUiHpC = registry.AddComponent<UIHealthComponent>(playerUi, DSFLOAT2(-0.8f, 0.8f), DSFLOAT2(1.0f, 1.0f));
	UIPlayerSoulsComponent* pcUiSC = registry.AddComponent<UIPlayerSoulsComponent>(playerUi, DSFLOAT2(-0.8f, 0.6f), DSFLOAT2(1.0f, 1.0f));

	//Setup stage to rotate around
	EntityID stage = registry.CreateEntity();
	// Stage Model
	ModelBonelessComponent* stageM = registry.AddComponent<ModelBonelessComponent>(stage);
	stageM->model = LoadModel("PlaceholderScene.mdl");
	// Stage Transform
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	// Stage POI
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);
	stageP->mode = POI_FORCE;
	stageP->height = CAMERA_OFFSET_Y * -0.85f;
	stageP->rotationY = 0.0f;
	stageP->rotationRadius = -0.7f * CAMERA_OFFSET_Z;
	stageP->rotationAccel = 0.12f;
	SetDirectionLight(1.0f, 1.0f, 1.0f, -1.0f, -1.0f, 1.0f);

	stateManager.activeLevel = 0;

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
	//Title
	auto title = registry.CreateEntity();
	auto tc = registry.AddComponent<UIImage>(title);
	tc->Setup("ExMenu/DamnedTitle3.png", { 0.0f, 0.20f }, { 1.0f, 1.0f});
	/*
	auto title2 = registry.CreateEntity();
	auto tc2 = registry.AddComponent<UIImage>(title2);
	tc2->Setup("ExMenu/DamnedTitle2.png", { 0.0f, 0.0f }, { 1.0f, 1.0f });

	auto title3 = registry.CreateEntity();
	auto tc3 = registry.AddComponent<UIImage>(title3);
	tc3->Setup("ExMenu/DamnedTitle3.png", { 0.0f, -0.25f }, { 1.0f, 1.0f });
	*/
/*
	//Eye 1
	auto eye1 = registry.CreateEntity();
	auto ec1 = registry.AddComponent<UIImage>(eye1);
	ec1->Setup("ExMenu/Eye.png", { -0.8f, 0.6f }, { 1.5f, 1.5f });

	//Eye 2
	auto eye2 = registry.CreateEntity();
	auto ec2 = registry.AddComponent<UIImage>(eye2);
	ec2->Setup("ExMenu/Eye.png", { 0.8f, 0.6f }, { 1.5f, 1.5f });
*/
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
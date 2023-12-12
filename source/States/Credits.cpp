#include "States/Credits.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "UI/UIButtonFunctions.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "UIComponents.h"
#include "Model.h"
#include "Levels\LevelHelper.h"

void Credits::Setup()
{
	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();

	Camera::ResetCamera();

	//Setup stage to rotate around
	// Stage Model
	StageSetupVariables stageVars;

	stageVars.stageNr = rand() % 10;
	EntityID stage = SetUpStage(stageVars);
	// Stage Transform
	TransformComponent* stageT = registry.AddComponent<TransformComponent>(stage);
	// Stage POI
	PointOfInterestComponent* stageP = registry.AddComponent<PointOfInterestComponent>(stage);
	stageP->mode = POI_FORCE;
	stageP->height = CAMERA_OFFSET_Y * -0.85f;
	stageP->rotationY = 0.0f;
	stageP->rotationRadius = -0.7f * CAMERA_OFFSET_Z;
	stageP->rotationAccel = 0.12f;
}

void Credits::Input()
{

}

void Credits::SetupButtons()
{
	auto button = registry.CreateEntity();
	OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

	uiElement->Setup("ButtonMedium", "Back", { 0.78f, -0.85f }, { 1.0f, 1.0f });

	onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::Credits_Back, UIFunctions::OnClick::None);
	onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::Image);

	SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
	buttonSound->Load(MENU);
}

void Credits::SetupImages()
{

}

void Credits::SetupText()
{
	const char courses[3][32] =
	{
		"Technical Artists:",
		"Master of Engineering:",
		"Game Programmers:"
	};

	const char TAnames[5][32] =
	{
		"Alexandra Lindberg",
		"Erik Svensson",
		"Erika Gustafsson",
		"Rasmus Fridlund",
		"Zannie Karlsson"
	};

	const char CIVnames[4][32] =
	{
		"Elliot Lundin",
		"Felix Mathiasson",
		"Herman Larsson",
		"Mattias Nordin"
	};

	const char SPnames[5][32] =
	{
		"Arian Watti",
		"Christian Marcuz",
		"Joaquin Lindkvist",
		"Joel Berg",
		"Niclas Andersson"
	};

	const DSFLOAT2 positions[3] =
	{
		{ 0.4f, 0.3f },
		{ 0.0f, 0.3f },
		{ -0.4f, 0.3f }
	};

	auto credits = registry.CreateEntity();
	UIComponent* uiElementC = registry.AddComponent<UIComponent>(credits);
	uiElementC->Setup("ButtonMedium", "Credits", DSFLOAT2( 0.0f, 0.6f ), DSFLOAT2(1.0f, 1.0f), 30.0f);
	uiElementC->m_BaseImage.baseUI.SetVisibility(true);

	for (int i = 0; i < 3; i++)
	{
		auto entity = registry.CreateEntity();
		UIComponent* uiElement = registry.AddComponent<UIComponent>(entity);
		switch (i)
		{
		case 0:
			uiElement->Setup("TempRelicFlavorHolder", courses[i], positions[i], DSFLOAT2(1.5f, 1.0f), 30.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			for (int j = 0; j < 5; j++)
			{
				uiElement->AddText(TAnames[j], uiElement->m_BaseImage.baseUI.GetBounds(), 
					{ uiElement->m_BaseImage.baseUI.GetPosition().x, uiElement->m_BaseImage.baseUI.GetPosition().y - (0.1f * (j + 2))}, DSFLOAT2(1.0f, 1.0f), 20.0f, 
					DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}
			break;
		case 1:
			uiElement->Setup("TempRelicFlavorHolder", courses[i], positions[i], DSFLOAT2(1.5f, 1.0f), 30.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			for (int j = 0; j < 4; j++)
			{
				uiElement->AddText(CIVnames[j], uiElement->m_BaseImage.baseUI.GetBounds(),
					{ uiElement->m_BaseImage.baseUI.GetPosition().x, uiElement->m_BaseImage.baseUI.GetPosition().y - (0.1f * (j + 2)) }, DSFLOAT2(1.0f, 1.0f), 20.0f,
					DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}
			break;
		case 2:
			uiElement->Setup("TempRelicFlavorHolder", courses[i], positions[i], DSFLOAT2(1.5f, 1.0f), 30.0f, DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			for (int j = 0; j < 5; j++)
			{
				uiElement->AddText(SPnames[j], uiElement->m_BaseImage.baseUI.GetBounds(),
					{ uiElement->m_BaseImage.baseUI.GetPosition().x, uiElement->m_BaseImage.baseUI.GetPosition().y - (0.1f * (j + 2)) }, DSFLOAT2(1.0f, 1.0f), 20.0f,
					DWRITE_TEXT_ALIGNMENT_CENTER, DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
			}
			break;
		default:
			break;
		}

		uiElement->m_BaseImage.baseUI.SetVisibility(false);
	}

}

void Credits::Unload()
{
	UnloadEntities();
}

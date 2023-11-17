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

void Credits::Setup()
{
	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();

	Camera::ResetCamera();

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

	uiElement->Setup("Exmenu/ButtonBackground", "\nBack", { -0.81f, -0.8f }, { 0.5f, 0.6f });

	onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), 1, UIFunc::Credits_Back);
	onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunc::HoverImage);

	SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
	buttonSound->Load(MENU);
}

void Credits::SetupImages()
{

}

void Credits::SetupText()
{
	const char const courses[3][32] =
	{
		"Technical Artists:",
		"Civil Engineers:",
		"Game Programmers:"
	};

	const char const TAnames[5][32] =
	{
		"Alexandra Lindberg",
		"Erik Svensson",
		"Erika Gustafsson",
		"Rasmus Fridlund",
		"Zannie"
	};

	const char const CIVnames[4][32] =
	{
		"Elliot Lundin",
		"Felix Mathiasson",
		"Herman Larsson",
		"Mattias Nordin"
	};

	const char const SPnames[5][32] =
	{
		"Arian Watti",
		"Christian Marcuz",
		"Joaquin Lindkvist",
		"Joel Berg",
		"Niclas Andersson"
	};

	const DSFLOAT2 const positions[3] =
	{
		{ 0.4f, 0.3f },
		{ 0.0f, 0.3f },
		{ -0.4f, 0.3f }
	};

	auto credits = registry.CreateEntity();
	UIComponent* uiElementC = registry.AddComponent<UIComponent>(credits);
	uiElementC->Setup("TempShopTitle", "Credits", { 0.0f, 0.6f });
	uiElementC->m_BaseImage.baseUI.SetVisibility(false);

	for (int i = 0; i < 3; i++)
	{
		auto entity = registry.CreateEntity();
		UIComponent* uiElement = registry.AddComponent<UIComponent>(entity);
		switch (i)
		{
		case 0:
			uiElement->Setup("TempRelicFlavorHolder", courses[i], positions[i], { 1.5f, 1.0f });
			for (int j = 0; j < 5; j++)
			{
				uiElement->AddText(TAnames[j], uiElement->m_BaseImage.baseUI.GetBounds(), 
					{ uiElement->m_BaseImage.baseUI.GetPosition().x, uiElement->m_BaseImage.baseUI.GetPosition().y - (0.1f * (j + 1))});
			}
			break;
		case 1:
			uiElement->Setup("TempRelicFlavorHolder", courses[i], positions[i], { 1.5f, 1.0f });
			for (int j = 0; j < 4; j++)
			{
				uiElement->AddText(CIVnames[j], uiElement->m_BaseImage.baseUI.GetBounds(),
					{ uiElement->m_BaseImage.baseUI.GetPosition().x, uiElement->m_BaseImage.baseUI.GetPosition().y - (0.1f * (j + 1)) });
			}
			break;
		case 2:
			uiElement->Setup("TempRelicFlavorHolder", courses[i], positions[i], { 1.5f, 1.0f });
			for (int j = 0; j < 5; j++)
			{
				uiElement->AddText(SPnames[j], uiElement->m_BaseImage.baseUI.GetBounds(),
					{ uiElement->m_BaseImage.baseUI.GetPosition().x, uiElement->m_BaseImage.baseUI.GetPosition().y - (0.1f * (j + 1)) });
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

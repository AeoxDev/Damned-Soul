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
#include "DeltaTime.h"

#include <fstream>
#include <string>

void Credits::Setup()
{
	RedrawUI();
	SetupUI();

	Camera::ResetCamera();
	Camera::SetCutsceneMode(0);

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
	gameSpeed = 1.0f;
}

void Credits::Input()
{

}

void Credits::SetupUI()
{
	const int people = 8;
	ML_String names[people][2] =
	{
		{"Alexandra Lindberg", "Arian Watti"},
		{"Christian Marcuz", "Elliot Lundin"},
		{"Erik Svensson", "Erika Gustafsson"},
		{"Felix Mathiasson", "Herman Larsson"},
		{"Joaquin Lindkvist", "Joel Berg"},
		{"Mattias Nordin", "Niclas Andersson"},
		{"Rasmus Fridlund", "Zannie Karlsson"},
		{"Special Thanks", "Blekinge Institute of Technology"}
	};

	auto credits = registry.CreateEntity();
	UIComponent* uiElementC = registry.AddComponent<UIComponent>(credits);
	uiElementC->Setup("Credits/BookNormal", "", "Credits", DSFLOAT2(0.0f, 0.0f), DSFLOAT2(1.0f, 1.0f), 40.0f);
	uiElementC->m_BaseText.baseUI.SetPosition({ 0.0f, 0.65f });

	uiElementC->m_BaseText.m_brush = Black;

	int reverse = 1;
	for (int i = 0; i < 2; i++)
	{
		std::ifstream file;
		ML_String filePath = "../resource/Credits/";
		filePath.append(names[stateManager.creditsIndex][i].c_str());
		filePath.append(".txt");

		file.open(filePath.c_str());

		std::string line;
		ML_String desc;
		if (file.is_open())
		{
			while (std::getline(file, line))
			{
				desc.append(line.c_str());
				desc.append("\n");
			}

			file.close();
		}

		uiElementC->AddText(names[stateManager.creditsIndex][i].c_str(), DSBOUNDS(0, 0, uiElementC->m_BaseImage.baseUI.GetBounds().right / 3.0f, uiElementC->m_BaseImage.baseUI.GetBounds().bottom),
			DSFLOAT2(-0.4 * reverse, 0.3), DSFLOAT2(1.0f, 1.0f), 40.0f);

		uiElementC->m_Texts[i + i].m_brush = Black;

		uiElementC->AddText(desc.c_str(), DSBOUNDS(0, 0, uiElementC->m_BaseImage.baseUI.GetBounds().right / 3.0f, uiElementC->m_BaseImage.baseUI.GetBounds().bottom),
			DSFLOAT2(-0.4 * reverse, -0.05), DSFLOAT2(1.0, 1.0), 20.0f, DWRITE_TEXT_ALIGNMENT_LEADING);

		uiElementC->m_Texts[i + i + 1].m_brush = Black;

		reverse = -1;
	}

	OnHoverComponent* onHoverC = registry.AddComponent<OnHoverComponent>(credits);
	OnClickComponent* onClickC = registry.AddComponent<OnClickComponent>(credits);

	uiElementC->AddImage("Credits/Arrow Left", DSFLOAT2(-0.55, -0.65), DSFLOAT2(1.0f, 1.0f), false);
	uiElementC->AddHoverImage(uiElementC->m_Images[uiElementC->m_Images.size() - 1], "Credits/Arrow Left Glow");

	onClickC->Setup(uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetPixelCoords(), uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetBounds(),
		UIFunctions::OnClick::None, UIFunctions::Credits::PreviousPage);

	onHoverC->Setup(uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetPixelCoords(),
		uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetBounds(), UIFunctions::OnHover::Image);

	uiElementC->AddImage("Credits/Arrow Right", DSFLOAT2(0.55, -0.65), DSFLOAT2(1.0f, 1.0f), false);
	uiElementC->AddHoverImage(uiElementC->m_Images[uiElementC->m_Images.size() - 1], "Credits/Arrow Right Glow");

	onClickC->Add(uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetPixelCoords(),
		uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetBounds(), UIFunctions::OnClick::None, UIFunctions::Credits::NextPage);

	onHoverC->Add(uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetPixelCoords(), 
		uiElementC->m_Images[uiElementC->m_Images.size() - 1].baseUI.GetBounds(), UIFunctions::OnHover::Image);

	registry.AddComponent<UICreditsTextComponent>(credits);

	auto button = registry.CreateEntity();
	OnClickComponent* onClick = registry.AddComponent<OnClickComponent>(button);
	OnHoverComponent* onHover = registry.AddComponent<OnHoverComponent>(button);
	UIComponent* uiElement = registry.AddComponent<UIComponent>(button);

	uiElement->Setup("ButtonMedium", "ButtonMediumHover", "Back", { 0.78f, -0.85f }, { 1.0f, 1.0f });

	onClick->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::Credits::Back, UIFunctions::OnClick::None);
	onHover->Setup(uiElement->m_BaseImage.baseUI.GetPixelCoords(), uiElement->m_BaseImage.baseUI.GetBounds(), UIFunctions::OnHover::Image);

	SoundComponent* buttonSound = registry.AddComponent<SoundComponent>(button);
	buttonSound->Load(MENU);
}

void Credits::Unload()
{
	UnloadEntities();
}

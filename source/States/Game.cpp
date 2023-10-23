#include "States\Game.h"
#include "DeltaTime.h"
#include "Camera.h"
#include "Model.h"
#include "GameRenderer.h"
#include "SDLHandler.h"
#include "D3D11Helper.h"
#include "Particles.h"
#include "Components.h"
#include "Input.h"
#include "States\StateManager.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "CollisionFunctions.h"
#include "EventFunctions.h"
#include "States\CleanupMacros.h"
#include "Camera.h"
#include "Light.h"
#include "UIComponents.h"

#include "RelicFunctions.h"

#include "MemLib\ML_String.hpp"

// Relic Stuff
#include "RelicFunctions.h"

void GameScene::Setup(int scene)//Load
{
	RedrawUI();	
	if (scene == 0)
	{
		// Set active
		m_active = true;
		
		//Setup Game HUD

		
		Camera::ResetCamera();
	}
}

void GameScene::Input()
{
	if (keyState[SDL_SCANCODE_ESCAPE] == pressed)
	{
		SetInMainMenu(true);
		SetInPlay(false);
		Unload();
		stateManager.menu.Setup();
	}

	if (keyState[SDL_SCANCODE_1] == pressed)
	{
		for (auto entity : View<RelicHolderComponent, UIPlayerRelicsComponent>(registry))
		{
			//auto relicHolder = registry.GetComponent<RelicHolderComponent>(entity);
			auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);

			if (uiElement->relics.size() > 8)
				break;

			//relicHolder->AddRelic<DamageRelic>();
			Relics::RelicMetaData md = Relics::SpeedyLittleDevil(true);

			UIImage relicImage, relicFlavorImage;
			relicImage.Setup(md.filePath);//("TempRelic1.png");
			relicFlavorImage.Setup("TempRelicFlavorHolder2.png");

			relicFlavorImage.m_UiComponent.SetVisibility(false);
			relicFlavorImage.m_UiComponent.SetScale({ 1.2f, 1.0f });

			UIText flavorTitle, flavorText;
			ML_String name = md.relicName;//relicHolder->GetRelic<DamageRelic>()->name;
			ML_String text = md.description;// relicHolder->GetRelic<DamageRelic>()->flavorText;

			std::wstring nameAsWString(name.begin(), name.end());
			std::wstring textAsWString(text.begin(), text.end());

			flavorTitle.Setup(nameAsWString);
			flavorText.Setup(textAsWString);

			flavorTitle.m_UiComponent.SetVisibility(false);
			flavorText.m_UiComponent.SetVisibility(false);

			UIRelicComponent relic({ 0.0f, 0.0f }, { 0.0f, 0.0f }, relicImage, relicFlavorImage, flavorTitle, flavorText);

			uiElement->relics.push_back(relic);

			RedrawUI();
		}
	}
	
	if (keyState[SDL_SCANCODE_2] == pressed)
	{
		for (auto entity : View<RelicHolderComponent, UIPlayerRelicsComponent>(registry))
		{
			auto relicHolder = registry.GetComponent<RelicHolderComponent>(entity);
			auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);
			
			if (uiElement->relics.size() > 8)
				break;

			relicHolder->AddRelic<SpeedRelic>();

			UIImage relicImage, relicFlavorImage;
			relicImage.Setup("TempRelic2.png");
			relicFlavorImage.Setup("TempRelicFlavorHolder2.png");

			relicFlavorImage.m_UiComponent.SetVisibility(false);
			relicFlavorImage.m_UiComponent.SetScale({ 1.2f, 1.0f });

			UIText flavorTitle, flavorText;
			ML_String name = relicHolder->GetRelic<SpeedRelic>()->name;
			ML_String text = relicHolder->GetRelic<SpeedRelic>()->flavorText;

			std::wstring nameAsWString(name.begin(), name.end());
			std::wstring textAsWString(text.begin(), text.end());

			flavorTitle.Setup(nameAsWString);
			flavorText.Setup(textAsWString);

			flavorTitle.m_UiComponent.SetVisibility(false);
			flavorText.m_UiComponent.SetVisibility(false);

			UIRelicComponent relic({ 0.0f, 0.0f }, { 0.0f, 0.0f }, relicImage, relicFlavorImage, flavorTitle, flavorText);

			uiElement->relics.push_back(relic);

			RedrawUI();
		}
	}

}

void GameScene::Update()
{
	
}

void GameScene::ComputeShaders()
{
	/*Particles::PrepareParticleCompute();
	Dispatch(100, 0, 0);
	Particles::FinishParticleCompute();*/
}

void GameScene::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	UnloadEntities();
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload();
	//Relics::ClearRelicFunctions();
	stateManager.menu.Setup();
}

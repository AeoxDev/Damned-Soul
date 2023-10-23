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

	for (auto entity : View<ModelBonelessComponent>(registry)) //So this gives us a view, or a mini-registry, containing every entity that has a ModelComponent
	{
		ModelBonelessComponent* dogCo = registry.GetComponent<ModelBonelessComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<ModelSkeletonComponent>(registry))
	{
		ModelSkeletonComponent* dogCo = registry.GetComponent<ModelSkeletonComponent>(entity);
		ReleaseModel(dogCo->model); // Decrement and potentially release via refcount
	}

	for (auto entity : View<UIPlayerSoulsComponent>(registry))
	{
		UIPlayerSoulsComponent* ps = registry.GetComponent<UIPlayerSoulsComponent>(entity);
		ps->image.Release();
	}

	for (auto entity : View<UIGameLevelComponent>(registry))
	{
		UIGameLevelComponent* ps = registry.GetComponent<UIGameLevelComponent>(entity);
		ps->image.Release();
	}

	for (auto entity : View<UIPlayerRelicsComponent>(registry))
	{
		UIPlayerRelicsComponent* r = registry.GetComponent<UIPlayerRelicsComponent>(entity);
		r->baseImage.Release();

		for (uint32_t i = 0; i < r->relics.size(); i++)
		{
			r->relics[i].sprite.Release();
			r->relics[i].flavorImage.Release();
		}

		r->relics.~ML_Vector();
	}

	for (auto entity : View<UIHealthComponent>(registry))
	{
		UIHealthComponent* ph = registry.GetComponent<UIHealthComponent>(entity);
		ph->backgroundImage.Release();
		ph->healthImage.Release();
	}

	for (auto entity : View<UIButton>(registry))
	{
		UIButton* b = registry.GetComponent<UIButton>(entity);
		b->Release();
	}

	for (auto entity : View<UIImage>(registry))
	{
		UIImage* i = registry.GetComponent<UIImage>(entity);
		i->Release();
	}

	for (auto entity : View<ProximityHitboxComponent>(registry))
	{
		ProximityHitboxComponent* p = registry.GetComponent<ProximityHitboxComponent>(entity);
		p->pointList.~ML_Vector();
	}

	Light::FreeLight();

	//Destroy entity resets component bitmasks
	for (int i = 0; i < registry.entities.size(); i++)
	{
		EntityID check = registry.entities.at(i).id;
		if(check.state == false)
			registry.DestroyEntity(check);
	}
}

void GameScene::GameOver()
{
	SetInMainMenu(true);
	SetInPlay(false);
	Unload();
	Relics::ClearRelicFunctions();
	stateManager.menu.Setup();
}

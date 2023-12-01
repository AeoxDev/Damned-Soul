#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Components.h"
#include "SDLHandler.h"
#include "Input.h"

#include "MemLib\ML_String.hpp"
#include "UIComponents.h"
#include "UI\HP_BarHelper.h"
#include <iomanip>

#include "States/StateManager.h"
#include "States/StateEnums.h"

bool uiUpdated = true;

void RedrawUI()
{
	uiUpdated = true;
}

bool UIRenderSystem::Update()
{
	if (uiUpdated)
	{
		uiUpdated = false;
		UpdateUI();
		Begin2dFrame(ui);

		for (auto entity : View<UIComponent>(registry))
		{
			auto uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->DrawAll();

		}

        End2dFrame(ui);
    }

	return true;
}

bool UIHealthSystem::Update()
{
	int counter = 0;
	for (auto entity : View<UIGameHealthComponent, UIComponent, StatComponent>(registry))
	{
		ScaleAdvancedHealthBar(entity);
//		auto health = registry.GetComponent<UIGameHealthComponent>(entity);
//		auto uiElement = registry.GetComponent<UIComponent>(entity);
//
//		auto stats = registry.GetComponent<StatComponent>(entity);
//		int64_t maxHealth = stats->GetMaxHealth();
//		int64_t currentHealth = stats->GetHealth();
//
//		float percentageHealth = (float)currentHealth / (float)maxHealth;
//
//		health->value = currentHealth;
//		int healthBoundsRight = (int)uiElement->m_Images[0].baseUI.m_OriginalBounds.right;
//
//		uiElement->m_Images[0].baseUI.m_CurrentBounds.right = healthBoundsRight * percentageHealth;
//
//		if (entity.index == stateManager.player.index)
//		{
//			uiElement->m_BaseText.SetText(("Health: " + std::to_string(health->value)).c_str(), uiElement->m_BaseText.baseUI.GetBounds());
//			uiElement->m_BaseText.baseUI.Setup(uiElement->m_BaseImage.baseUI.GetPosition(), uiElement->m_BaseImage.baseUI.GetScale(),
//				uiElement->m_BaseImage.baseUI.GetRotation(), uiElement->m_BaseImage.baseUI.GetVisibility(), uiElement->m_BaseImage.baseUI.GetOpacity());
//		}
//		else
//		{
//#ifdef DEBUG_HP
//			TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
//
//			uiElement->m_Images[0].baseUI.SetPosition(DSFLOAT2(0.75f, 0.75f - (counter * 0.125f)));
//
//			uiElement->m_BaseText.SetText(("Health: " + std::to_string(health->value)).c_str(), uiElement->m_Images[0].baseUI.GetBounds());
//			uiElement->m_BaseText.baseUI.Setup(uiElement->m_Images[0].baseUI.GetPosition(), uiElement->m_Images[0].baseUI.GetScale(),
//				uiElement->m_Images[0].baseUI.GetRotation(), uiElement->m_Images[0].baseUI.GetVisibility(), uiElement->m_Images[0].baseUI.GetOpacity());
//
//
//			counter++;
//#endif
//			
//		}
	}

	return true;
}

bool UIPlayerSoulsSystem::Update()
{
	for (auto entity : View<PlayerComponent, UIPlayerSoulsComponent, UIComponent>(registry))
		UpdateSoulUI(entity);
	//int currentSouls = 0;

	//for (auto entity : View<PlayerComponent>(registry))
	//{
	//	auto player = registry.GetComponent<PlayerComponent>(entity);
	//	currentSouls = player->GetSouls();
	//}

 //   for (auto entity : View<UIPlayerSoulsComponent, UIComponent>(registry))
 //   {
	//	auto souls = registry.GetComponent<UIPlayerSoulsComponent>(entity);
	//	auto uiElement = registry.GetComponent<UIComponent>(entity);

	//	souls->value = currentSouls;
	//	
	//	if (uiElement->m_Texts.size() > 0)
	//	{
	//		uiElement->m_Texts[0].SetText(("Souls: " + std::to_string(souls->value)).c_str(), uiElement->m_Texts[0].baseUI.GetBounds());
	//		uiElement->m_Texts[0].baseUI.Setup(uiElement->m_Images[1].baseUI.GetPosition(), uiElement->m_Images[1].baseUI.GetScale(),
	//			uiElement->m_Images[1].baseUI.GetRotation(), uiElement->m_Images[1].baseUI.GetVisibility(), uiElement->m_Images[1].baseUI.GetOpacity());
	//	}
	//}

	return true;
}

bool UIShopSystem::Update()
{
	if (!(currentStates & State::InShop))
	{
		for (auto entity : View<UIShopImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(false);
		}

		for (auto entity : View<UIShopTitleImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(false);
		}

		for (auto entity : View<UIShopRelicComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(false);
		}

		for (auto entity : View<UIShopRerollComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(false);
		}

		for (auto entity : View<UIShopButtonComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(false);
		}
	}
	else
	{
		for (auto entity : View<UIShopImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(true);
			//uiElement->m_BaseImage.baseUI.SetVisibility(false);
		}

		for (auto entity : View<UIShopTitleImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(true);
			uiElement->m_BaseImage.baseUI.SetVisibility(false);
		}

		for (auto entity : View<UIShopRelicComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

			uiElement->m_BaseImage.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Images.size() - (uiElement->m_Images.size() / 2); i++)
				uiElement->m_Images[i].baseUI.SetVisibility(true);

			uiElement->m_BaseText.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Texts.size(); i++)
				uiElement->m_Texts[i].baseUI.SetVisibility(true);
		}

		for (auto entity : View<UIShopRerollComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(true);
		}

		for (auto entity : View<UIShopButtonComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(true);
		}
	}

    return true;
}


bool UIRunTimeSystem::Update()
{

	for (auto entity : View<UIGameTimeComponent, UIComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UIGameTimeComponent* runTime = registry.GetComponent<UIGameTimeComponent>(entity);
		ML_String clock = GetDigitalMinuteClock();
		uiElement->m_BaseText.SetText(clock.c_str(), DSBOUNDS(0.0f, 0.0f, 0.0f, 0.0f));
	}
	return true;
}

bool UISliderSystem::Update()
{
	AudioEngineComponent* audioComp = nullptr;

	for (auto audio : View<AudioEngineComponent>(registry))
		audioComp = registry.GetComponent<AudioEngineComponent>(audio);

	for (auto entity : View<UISettingsSliderComponent, UIComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UISettingsSliderComponent* slider = registry.GetComponent<UISettingsSliderComponent>(entity);

		float uiMouseCoords = (((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)) / (0.5f * sdl.BASE_WIDTH)) - 1.0f;

		if (slider->holding)
		{

			float maxLeftPosition = uiElement->m_BaseImage.baseUI.GetPositionBounds().left;
			float maxRightPosition = uiElement->m_BaseImage.baseUI.GetPositionBounds().right;

			float sliderWidth = abs(maxRightPosition - 0.13f) - abs(maxLeftPosition + 0.13f);

			if (slider->currentPosition >= maxRightPosition - 0.13f)
				slider->currentPosition = maxRightPosition - 0.13f;

			if (slider->currentPosition <= maxLeftPosition + 0.13f)
				slider->currentPosition = maxLeftPosition + 0.13f;

			if (uiMouseCoords > slider->currentPosition && slider->currentPosition < maxRightPosition - 0.13f)
			{
				slider->currentPercentage += 0.01f;
				slider->currentPosition += (sliderWidth * 0.01f);

				RedrawUI();
			}

			if (uiMouseCoords < slider->currentPosition && slider->currentPosition > maxLeftPosition + 0.13f)
			{
				slider->currentPercentage -= 0.01f;
				slider->currentPosition -= (sliderWidth * 0.01f);

				RedrawUI();
			}

			if (audioComp != nullptr)
			{
				if (uiElement->m_BaseText.m_Text == "Master Volume")
				{
					audioComp->groups[MASTER_GROUP]->setVolume(slider->currentPercentage);
				}
				if (uiElement->m_BaseText.m_Text == "Voice Volume")
				{
					audioComp->groups[VOICE_GROUP]->setVolume(slider->currentPercentage);
				}
				if (uiElement->m_BaseText.m_Text == "SFX Volume")
				{
					audioComp->groups[SFX_GROUP]->setVolume(slider->currentPercentage);
				}
				if (uiElement->m_BaseText.m_Text == "Music Volume")
				{
					audioComp->groups[MUSIC_GROUP]->setVolume(slider->currentPercentage);
				}
				if (uiElement->m_BaseText.m_Text == "Ambient Volume")
				{
					audioComp->groups[AMBIENCE_GROUP]->setVolume(slider->currentPercentage);
				}

			}
		}

		uiElement->m_Images[0].baseUI.SetPosition(DSFLOAT2(slider->currentPosition, uiElement->m_Images[0].baseUI.GetPosition().y));
	}

	return true;
}
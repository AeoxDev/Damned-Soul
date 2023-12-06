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
#include "Relics/Utility/RelicFuncInputTypes.h"

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
	for (auto entity : View<UIGameHealthComponent, UIComponent, StatComponent>(registry))
		ScaleAdvancedHealthBar(entity);

	return true;
}

bool UIPlayerSoulsSystem::Update()
{
	for (auto entity : View<PlayerComponent, UIPlayerSoulsComponent, UIComponent>(registry))
		UpdateSoulUI(entity);

	return true;
}

bool UIRenderShopSystem::Update()
{
	if (!(currentStates & State::InShop))
	{

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

		for (auto entity : View<UIShopPlayerStatsComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			StatComponent* player = registry.GetComponent<StatComponent>(stateManager.player);

			char atkDmgStr[64] = "";
			sprintf(atkDmgStr, "Damage: %.1lf + (%.1lf)", player->GetBaseDamage(), player->GetBonusDamage());
			uiElement->m_Texts[0].SetText(atkDmgStr, uiElement->m_BaseImage.baseUI.GetBounds());

			char atkSpdStr[64] = "";
			sprintf(atkSpdStr, "Attack Speed: %.0lf%%", player->GetAttackSpeed() * 100.f);
			uiElement->m_Texts[1].SetText(atkSpdStr, uiElement->m_BaseImage.baseUI.GetBounds());

			char movSpeed[64] = "";
			player->SetSpeedMult(1.f);
			sprintf(movSpeed, "Movement Speed: %.0lf%%", (player->GetSpeed() / player->GetBaseSpeed()) * 100.f);
			player->SetSpeedMult(0.f);
			uiElement->m_Texts[2].SetText(movSpeed, uiElement->m_BaseImage.baseUI.GetBounds());
		}

		RelicInput::OnPriceCalculation priceCalc;

		for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
			func(&priceCalc);

		for (auto entity : View<UIShopRelicComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			UIShopRelicComponent* relicWindow = registry.GetComponent<UIShopRelicComponent>(entity);
			PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
			uiElement->m_BaseImage.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Images.size() - (uiElement->m_Images.size() / 2); i++)
			{
				if (priceCalc.GetCostOf(relicWindow->shopRelics[i]->m_price, RelicInput::OnPriceCalculation::RELIC) > player->GetSouls())
					uiElement->m_Images[i].baseUI.SetOpacity(0.3f);
				else
					uiElement->m_Images[i].baseUI.SetOpacity(1.0f);

				uiElement->m_Images[i].baseUI.SetVisibility(true);
			}

			uiElement->m_BaseText.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Texts.size(); i++)
			{
				uiElement->m_Texts[i].baseUI.SetVisibility(true);
				uiElement->m_Texts[i].SetText(std::to_string(priceCalc.GetCostOf(relicWindow->shopRelics[i]->m_price, RelicInput::OnPriceCalculation::RELIC)).c_str(), uiElement->m_Images[i].baseUI.GetBounds());
			}
		}

		for (auto entity : View<UIShopRerollComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->SetAllVisability(true);
		}

		for (auto entity : View<UIShopUpgradeComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			UIShopButtonComponent* button = registry.GetComponent<UIShopButtonComponent>(entity);
			UIShopUpgradeComponent* upgrade = registry.GetComponent<UIShopUpgradeComponent>(entity);
			if (upgrade->tier == 2)
			{
				button->m_description = "Fully Upgraded";
				button->m_price = 666;
			}
		}

		for (auto entity : View<UIShopButtonComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			UIShopButtonComponent* button = registry.GetComponent<UIShopButtonComponent>(entity);
			uiElement->SetAllVisability(true);

			if (button->m_name == "Heal")
			{
				uiElement->m_Texts[0].SetText(std::to_string(priceCalc.GetCostOf(button->m_price, RelicInput::OnPriceCalculation::HEAL)).c_str(), uiElement->m_Images[0].baseUI.GetBounds());
			}
			else if (button->m_name == "Reroll")
			{
				uiElement->m_Texts[0].SetText(std::to_string(priceCalc.GetCostOf(button->m_price, RelicInput::OnPriceCalculation::REROLL)).c_str(), uiElement->m_Images[0].baseUI.GetBounds());
			}
			else if (button->m_name == "Upgrade Weapon")
			{
				uiElement->m_Texts[0].SetText(std::to_string(priceCalc.GetCostOf(button->m_price, RelicInput::OnPriceCalculation::UPGRADE)).c_str(), uiElement->m_Images[0].baseUI.GetBounds());
			}
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

			float sliderWidth = abs(maxRightPosition - 0.13f) + abs(maxLeftPosition + 0.13f);

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
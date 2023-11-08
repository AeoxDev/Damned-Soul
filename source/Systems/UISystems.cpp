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
            uiElement->m_BaseImage.Draw();

			for (UIImage image : uiElement->m_Images)
				image.Draw();

            uiElement->m_Text.Draw();
		}

        End2dFrame(ui);
    }

	return true;
}

bool UIHealthSystem::Update()
{
	DSFLOAT2 healthScale = {0,0};
	int healthBoundsRight = 0;
	float percentageHealth = 0.0f, currentHealth = 0.0f;

	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		auto stats = registry.GetComponent<StatComponent>(entity);
		auto maxHealth = stats->GetMaxHealth();
		currentHealth = stats->GetHealth();

		percentageHealth = currentHealth / maxHealth;
	}

	for (auto entity : View<UIHealthComponent, UIComponent>(registry))
	{
		auto health = registry.GetComponent<UIHealthComponent>(entity);
		auto uiElement = registry.GetComponent<UIComponent>(entity);

		health->value = currentHealth;
		healthScale = uiElement->m_BaseImage.baseUI.GetScale();
		int healthBoundsRight = (int)uiElement->m_Images[0].baseUI.m_OriginalBounds.right;

		uiElement->m_Images[0].baseUI.m_CurrentBounds.right = healthBoundsRight * percentageHealth;

		uiElement->m_Text.SetText(("Health: " + std::to_string((int)health->value)).c_str(), uiElement->m_Text.baseUI.GetBounds());
		uiElement->m_Text.baseUI.Setup(uiElement->m_BaseImage.baseUI.GetPosition(), uiElement->m_BaseImage.baseUI.GetScale(), 
			uiElement->m_BaseImage.baseUI.GetRotation(), true, uiElement->m_BaseImage.baseUI.GetOpacity());
	}

	return true;
}

bool UIPlayerSoulsSystem::Update()
{
	int currentSouls = 0;

	for (auto entity : View<PlayerComponent>(registry))
	{
		auto player = registry.GetComponent<PlayerComponent>(entity);
		currentSouls = player->GetSouls();
	}

    for (auto entity : View<UIPlayerSoulsComponent, UIComponent>(registry))
    {
		auto souls = registry.GetComponent<UIPlayerSoulsComponent>(entity);
		auto uiElement = registry.GetComponent<UIComponent>(entity);

		souls->value = currentSouls;

		uiElement->m_Text.SetText(("Souls: " + std::to_string((int)souls->value)).c_str(), uiElement->m_Text.baseUI.GetBounds());
		uiElement->m_Text.baseUI.Setup(uiElement->m_BaseImage.baseUI.GetPosition(), uiElement->m_BaseImage.baseUI.GetScale(),
			uiElement->m_BaseImage.baseUI.GetRotation(), true, uiElement->m_BaseImage.baseUI.GetOpacity());
	}

	return true;
}

bool UIRelicsSystem::Update()
{

    return true;
}

bool UIShopSystem::Update()
{
	if (!(currentStates & State::InShop))
	{
		for (auto entity : View<UIShopImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			
			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(false);
			}

			uiElement->m_Text.baseUI.SetVisibility(false);
		}

		for (auto entity : View<UIShopTitleImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(false);
			}

			uiElement->m_Text.baseUI.SetVisibility(false);
		}

		for (auto entity : View<UIRelicWindowComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->m_BaseImage.baseUI.SetVisibility(false);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(false);
			}

			uiElement->m_Text.baseUI.SetVisibility(false);
		}

		for (auto entity : View<UIRerollComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->m_BaseImage.baseUI.SetVisibility(false);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(false);
			}

			uiElement->m_Text.baseUI.SetVisibility(false);
		}

		for (auto entity : View<UIShopButtonComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->m_BaseImage.baseUI.SetVisibility(false);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(false);
			}

			uiElement->m_Text.baseUI.SetVisibility(false);
		}
	}
	else
	{
		for (auto entity : View<UIShopImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(true);
			}

			uiElement->m_Text.baseUI.SetVisibility(true);
		}

		for (auto entity : View<UIShopTitleImpComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(true);
			}

			uiElement->m_Text.baseUI.SetVisibility(true);
		}

		for (auto entity : View<UIRelicWindowComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->m_BaseImage.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Images.size() - (uiElement->m_Images.size() / 2); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(true);
			}

			uiElement->m_Text.baseUI.SetVisibility(true);
		}

		for (auto entity : View<UIRerollComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->m_BaseImage.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(true);
			}

			uiElement->m_Text.baseUI.SetVisibility(true);
		}

		for (auto entity : View<UIShopButtonComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			uiElement->m_BaseImage.baseUI.SetVisibility(true);

			for (UINT32 i = 0; i < uiElement->m_Images.size(); i++)
			{
				uiElement->m_Images[i].baseUI.SetVisibility(true);
			}

			uiElement->m_Text.baseUI.SetVisibility(true);
		}
	}

    return true;
}
#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Components.h"
#include "SDLHandler.h"
#include "Input.h"

#include "MemLib\ML_String.hpp"

#include <iostream>
#include <iomanip>

bool uiUpdated = true;

void RedrawUI()
{
    uiUpdated = true;
}

void SetTextAndImageProperties(ML_String, UIText&, UIImage&, DirectX::XMFLOAT2, DirectX::XMFLOAT2);

bool UIRenderSystem::Update()
{
    if (uiUpdated)
    {
        uiUpdated = false;
        UpdateUI();
        Begin2dFrame(ui);

        for (auto entity : View<UIButton>(registry))
            registry.GetComponent<UIButton>(entity)->Draw();

        for (auto entity : View<UIImage>(registry))
            registry.GetComponent<UIImage>(entity)->Draw();

        for (auto entity : View<UIPlayerRelicsComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);
            uiElement->baseImage.Draw();

            for (uint32_t i = 0; i < uiElement->relics.size(); i++)
            {
                uiElement->relics[i].sprite.Draw();
                uiElement->relics[i].flavorImage.Draw();
                uiElement->relics[i].flavorTitle.Draw();
                uiElement->relics[i].flavorText.Draw();
            }

        }

        for (auto entity : View<UIText>(registry))
            registry.GetComponent<UIText>(entity)->Draw();

        for (auto entity : View<UIGameLevelComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIGameLevelComponent>(entity);
            uiElement->image.Draw();
            uiElement->text.Draw();
        }

        for (auto entity : View<UIHealthComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIHealthComponent>(entity);
            uiElement->image.Draw();
            uiElement->text.Draw();
        }

        for (auto entity : View<UIPlayerSoulsComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIPlayerSoulsComponent>(entity);
            uiElement->image.Draw();
            uiElement->text.Draw();
        }

        End2dFrame(ui);
    }

    return true;
}

bool UIHealthSystem::Update()
{
    for (auto entity : View<UIHealthComponent, StatComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIHealthComponent>(entity);
        auto stats = registry.GetComponent<StatComponent>(entity);
        uiElement->value = stats->health;

        ML_String valueAsString = ("Health: " + std::to_string((int)uiElement->value)).c_str();
        SetTextAndImageProperties(valueAsString, uiElement->text, uiElement->image, uiElement->scale, uiElement->position);
    }

    return true;
}

bool UIPlayerSoulsSystem::Update()
{
    for (auto entity : View<UIPlayerSoulsComponent, PlayerComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIPlayerSoulsComponent>(entity);
        auto player = registry.GetComponent<PlayerComponent>(entity);
        uiElement->value = player->souls;

        ML_String valueAsString = ("Souls: " + std::to_string(uiElement->value)).c_str();
        SetTextAndImageProperties(valueAsString, uiElement->text, uiElement->image, uiElement->scale, uiElement->position);
    }

    return true;
}

bool UIPlayerRelicsSystem::Update()
{
    for (auto entity : View<UIPlayerRelicsComponent, RelicHolderComponent>(registry))
    {
        auto relicHolder = registry.GetComponent<RelicHolderComponent>(entity);
        auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);

        uiElement->baseImage.m_UiComponent.SetScale(uiElement->scale);
        uiElement->baseImage.m_UiComponent.SetPosition(uiElement->position);

        if (uiElement->relics.size() == 0)
            return true;

        DirectX::XMFLOAT2 baseImagePosition = { abs(uiElement->baseImage.m_UiComponent.GetPosition().x + uiElement->baseImage.m_UiComponent.m_CurrentBounds.right / 18) ,
                                       abs(uiElement->baseImage.m_UiComponent.GetPosition().y + uiElement->baseImage.m_UiComponent.m_CurrentBounds.bottom / 2) };
        DirectX::XMFLOAT2 baseImagePixelCoords = { (baseImagePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
                                        -1 * ((baseImagePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

        if (uiElement->relicIndex == uiElement->relics.size() - 1)
        {
            uiElement->relics[uiElement->relicIndex].sprite.m_UiComponent.SetScale({ 1.0f , 1.0f });

            /*if (uiElement->relicIndex % 3 == 0 && uiElement->relicIndex != 0)
            {
                uiElement->gridPosition.y++;
                uiElement->gridPosition.x = 0;
            }*/

            uiElement->relics[uiElement->relicIndex].flavorImage.m_UiComponent.SetPosition({ baseImagePixelCoords.x + (0.1f * uiElement->gridPosition.x), baseImagePixelCoords.y - (0.15f * uiElement->gridPosition.y) });
            uiElement->relics[uiElement->relicIndex].flavorTitle.m_UiComponent.SetPosition({ baseImagePixelCoords.x + (0.1f * uiElement->gridPosition.x), baseImagePixelCoords.y - (0.15f * uiElement->gridPosition.y) });
            uiElement->relics[uiElement->relicIndex].flavorText.m_UiComponent.SetPosition({ baseImagePixelCoords.x + (0.1f * uiElement->gridPosition.x), baseImagePixelCoords.y - (0.15f * uiElement->gridPosition.y) });

            uiElement->relics[uiElement->relicIndex].sprite.m_UiComponent.SetPosition({ baseImagePixelCoords.x + (0.1f * uiElement->gridPosition.x), baseImagePixelCoords.y - (0.15f * uiElement->gridPosition.y) });
            uiElement->relicIndex++;
            uiElement->gridPosition.x++;

            RedrawUI();
        } 

        for (size_t i = 0; i < uiElement->relics.size(); i++)
        {
            if (uiElement->relics[i].sprite.m_UiComponent.Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
            {

                uiElement->relics[i].flavorImage.m_UiComponent.SetVisibility(true);
                uiElement->relics[i].flavorTitle.m_UiComponent.SetVisibility(true);
                //uiElement->relics[i].flavorText.m_UiComponent.SetVisibility(true);

                if (uiElement->relics[i].flavorImage.m_UiComponent.IsVisible() && uiElement->relics[i].doRedraw)
                {
                    RedrawUI();
                    uiElement->relics[i].doRedraw = false;
                }

            }
            else
            {
                if (uiElement->relics[i].flavorImage.m_UiComponent.IsVisible())
                {
                    RedrawUI();
                    uiElement->relics[i].flavorImage.m_UiComponent.SetVisibility(false);
                    uiElement->relics[i].flavorTitle.m_UiComponent.SetVisibility(false);
                    //uiElement->relics[i].flavorText.m_UiComponent.SetVisibility(false);
                    uiElement->relics[i].doRedraw = true;
                }


            }
        }
    }

    return true;
}

bool UIGameLevelSystem::Update()
{
    for (auto entity : View<UIGameLevelComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIGameLevelComponent>(entity);

        ML_String valueAsString = std::to_string(uiElement->value).c_str();
        SetTextAndImageProperties(valueAsString, uiElement->text, uiElement->image, uiElement->scale, uiElement->position);
    }
    return true;
}

void SetTextAndImageProperties(ML_String text, UIText& uiText, UIImage& uiImage, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 position)
{
   
    std::wstring valueAsWString(text.begin(), text.end());

    uiText.UpdateText(valueAsWString);

    uiText.m_UiComponent.SetScale(scale);
    uiText.m_UiComponent.SetPosition(position);

    uiImage.m_UiComponent.SetScale(scale);
    uiImage.m_UiComponent.SetPosition(position);

}

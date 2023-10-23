#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Components.h"
#include "SDLHandler.h"
#include "Input.h"

#include "MemLib\ML_String.hpp"

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

        for (auto entity : View<UIImage>(registry))
            registry.GetComponent<UIImage>(entity)->Draw();

        for (auto entity : View<UIPlayerRelicsComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);
            uiElement->baseImage.Draw();

            /*for (uint32_t i = 0; i < uiElement->relics.size(); i++)
                uiElement->relics[i].sprite.Draw();

            for (uint32_t i = 0; i < uiElement->relics.size(); i++)
                uiElement->relics[i].flavorTitleImage.Draw();

            for (uint32_t i = 0; i < uiElement->relics.size(); i++)
                uiElement->relics[i].flavorTitle.Draw();*/
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

        for (auto entity : View<UIShopRelicWindowComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
            uiElement->m_baseImage.Draw();
            uiElement->m_priceText.Draw();
        }

        for (auto entity : View<UIRelicComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIRelicComponent>(entity);
            uiElement->sprite.Draw();
            uiElement->flavorTitleImage.Draw();
            uiElement->flavorTitle.Draw();
            uiElement->flavorDescImage.Draw();
            uiElement->flavorDesc.Draw();
        }

        for (auto entity : View<UIShopComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIShopComponent>(entity);
            uiElement->baseImage.Draw();
            uiElement->playerInfo.Draw();
        }

        for (auto entity : View<UIButton>(registry))
            registry.GetComponent<UIButton>(entity)->Draw();

        End2dFrame(ui);
    }

    return true;
}

bool UIHealthSystem::Update()
{
    EntityID playerUI;
    for (auto entity : View<UIHealthComponent, UIPlayerSoulsComponent>(registry))
        playerUI = entity;

    for (auto entity : View<StatComponent, PlayerComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIHealthComponent>(playerUI);
        auto stats = registry.GetComponent<StatComponent>(entity);
        uiElement->value = stats->GetHealth();

        ML_String valueAsString = ("Health: " + std::to_string((int)uiElement->value)).c_str();
        SetTextAndImageProperties(valueAsString, uiElement->text, uiElement->image, uiElement->scale, uiElement->position);
    }

    return true;
}

bool UIPlayerSoulsSystem::Update()
{
    EntityID playerUI;
    for (auto entity : View<UIHealthComponent, UIPlayerSoulsComponent>(registry))
        playerUI = entity;

    for (auto entity : View<PlayerComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIPlayerSoulsComponent>(playerUI);
        auto player = registry.GetComponent<PlayerComponent>(entity);
        uiElement->value = player->GetSouls();

        ML_String valueAsString = ("Souls: " + std::to_string(uiElement->value)).c_str();
        SetTextAndImageProperties(valueAsString, uiElement->text, uiElement->image, uiElement->scale, uiElement->position);
    }

    return true;
}

bool UIRelicsSystem::Update()
{
    for (auto entity : View<UIPlayerRelicsComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);

        uiElement->baseImage.m_UiComponent.SetScale(uiElement->scale);
        uiElement->baseImage.m_UiComponent.SetPosition(uiElement->position);

        if (uiElement->relics.size() == 0)
            return true;

        DirectX::XMFLOAT2 spritePositionOffset = { uiElement->baseImage.m_UiComponent.m_CurrentBounds.right/ (uiElement->baseImage.m_UiComponent.m_CurrentBounds.right / 40.0f) ,
                                                uiElement->baseImage.m_UiComponent.m_CurrentBounds.bottom /(uiElement->baseImage.m_UiComponent.m_CurrentBounds.bottom / 40.0f)};

        DirectX::XMFLOAT2 startingSpritePosition = { abs(uiElement->baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
                                       abs(uiElement->baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
        DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
                                        -1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

        if (uiElement->relicIndex == uiElement->relics.size() - 1)
        {
            uiElement->relics[uiElement->relicIndex].sprite.m_UiComponent.SetScale({ 1.0f , 1.0f });

            /*if (uiElement->relicIndex % 3 == 0 && uiElement->relicIndex != 0)
            {
                uiElement->gridPosition.y++;
                uiElement->gridPosition.x = 0;
            }*/

            uiElement->relics[uiElement->relicIndex].flavorTitleImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * (uiElement->gridPosition.y + 1.25f)) });
            uiElement->relics[uiElement->relicIndex].flavorTitle.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * (uiElement->gridPosition.y + 1.25f)) });
            uiElement->relics[uiElement->relicIndex].flavorTitleImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * (uiElement->gridPosition.y) + 1.25f) });

            uiElement->relics[uiElement->relicIndex].sprite.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * uiElement->gridPosition.y) });
            uiElement->relicIndex++;
            uiElement->gridPosition.x++;

            RedrawUI();
        } 

        for (uint32_t i = 0; i < uiElement->relics.size(); i++)
        {
            if (uiElement->relics[i].sprite.m_UiComponent.Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
            {

                uiElement->relics[i].flavorTitleImage.m_UiComponent.SetVisibility(true);
                uiElement->relics[i].flavorTitle.m_UiComponent.SetVisibility(true);
                //uiElement->relics[i].flavorDesc.m_UiComponent.SetVisibility(true);

                if (uiElement->relics[i].flavorTitleImage.m_UiComponent.IsVisible() && uiElement->relics[i].doRedraw)
                {
                    RedrawUI();
                    uiElement->relics[i].doRedraw = false;
                }

            }
            else
            {
                if (uiElement->relics[i].flavorTitleImage.m_UiComponent.IsVisible())
                {
                    RedrawUI();
                    uiElement->relics[i].flavorTitleImage.m_UiComponent.SetVisibility(false);
                    uiElement->relics[i].flavorTitle.m_UiComponent.SetVisibility(false);
                    //uiElement->relics[i].flavorDesc.m_UiComponent.SetVisibility(false);
                    uiElement->relics[i].doRedraw = true;
                }


            }
        }
    }

    for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
        auto uiRelic = registry.GetComponent<UIRelicComponent>(entity);

        if (uiRelic->sprite.m_UiComponent.Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
        {
            uiRelic->flavorTitleImage.m_UiComponent.SetVisibility(true);
            uiRelic->flavorTitle.m_UiComponent.SetVisibility(true);
            uiRelic->flavorDescImage.m_UiComponent.SetVisibility(true);
            uiRelic->flavorDesc.m_UiComponent.SetVisibility(true);

            if (uiRelic->flavorTitleImage.m_UiComponent.IsVisible() && uiRelic->doRedraw)
            {
                RedrawUI();
                uiRelic->doRedraw = false;
            }
        }
        else
        {
            if (uiRelic->flavorTitleImage.m_UiComponent.IsVisible())
            {
                RedrawUI();
                uiRelic->flavorTitleImage.m_UiComponent.SetVisibility(false);
                uiRelic->flavorTitle.m_UiComponent.SetVisibility(false);
                uiRelic->flavorDescImage.m_UiComponent.SetVisibility(false);
                uiRelic->flavorDesc.m_UiComponent.SetVisibility(false);
                uiRelic->doRedraw = true;
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

bool UIShopSystem::Update()
{
    StatComponent* stats = nullptr;
    PlayerComponent* player = nullptr;

    for (auto entity : View<PlayerComponent, StatComponent>(registry))
    {
        stats = registry.GetComponent<StatComponent>(entity);
        player = registry.GetComponent<PlayerComponent>(entity);
    }

    for (auto entity : View<UIShopComponent>(registry))
    {
        auto uiShopElement = registry.GetComponent<UIShopComponent>(entity);

        ML_String playerInfo = ("Souls: " + std::to_string((int)player->GetSouls()) +
            "\n\nPlayer Stats:\nHealth: " + std::to_string((int)stats->GetHealth()) +
            "\nDamage: " + std::to_string((int)stats->damage) + 
            "\nMove Speed: " + std::to_string((int)stats->moveSpeed) +
            "\nAttack Speed: " + std::to_string((int)stats->attackSpeed)).c_str(); // Warning gets to stay for now


        DirectX::XMFLOAT2 spritePositionOffset = { uiShopElement->baseImage.m_UiComponent.m_CurrentBounds.right / (uiShopElement->baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
                                               uiShopElement->baseImage.m_UiComponent.m_CurrentBounds.bottom / (uiShopElement->baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

        DirectX::XMFLOAT2 startingSpritePosition = { abs(uiShopElement->baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
                                       abs(uiShopElement->baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
        DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
                                        -1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };
        
        uiShopElement->playerInfo.UpdateText(playerInfo);
        uiShopElement->playerInfo.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1), spritePixelCoords.y - (0.1f * 2) });

    }

    return true;
}

void SetTextAndImageProperties(ML_String text, UIText& uiText, UIImage& uiImage, DirectX::XMFLOAT2 scale, DirectX::XMFLOAT2 position)
{

    uiText.UpdateText(text);

    uiText.m_UiComponent.SetScale(scale);
    uiText.m_UiComponent.SetPosition(position);

    uiImage.m_UiComponent.SetScale(scale);
    uiImage.m_UiComponent.SetPosition(position);

}

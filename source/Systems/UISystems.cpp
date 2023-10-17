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

        for (auto entity : View<UIButton>(registry))
            registry.GetComponent<UIButton>(entity)->Draw();

        for (auto entity : View<UIImage>(registry))
            registry.GetComponent<UIImage>(entity)->Draw();

        for (auto entity : View<UIPlayerRelicsComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIPlayerRelicsComponent>(entity);
            uiElement->baseImage.Draw();

            for (uint32_t i = 0; i < uiElement->relics.size(); i++)
                uiElement->relics[i].sprite.Draw();

            for (uint32_t i = 0; i < uiElement->relics.size(); i++)
                uiElement->relics[i].flavorImage.Draw();

            for (uint32_t i = 0; i < uiElement->relics.size(); i++)
                uiElement->relics[i].flavorTitle.Draw();
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

        for (auto entity : View<UIShopComponent>(registry))
        {
            auto uiElement = registry.GetComponent<UIShopComponent>(entity);
            uiElement->baseImage.Draw();
            uiElement->playerInfo.Draw();
            uiElement->rerollButton.Draw();
            
            for (uint32_t i = 0; i < uiElement->relicWindows.size(); i++)
            {
                uiElement->relicWindows[i].m_baseImage.Draw();
                uiElement->relicWindows[i].m_relicImage.Draw();
                uiElement->relicWindows[i].m_relicName.Draw();

                for (uint32_t j = 0; j < uiElement->relicWindows[i].m_buttons.size(); j++)
                    uiElement->relicWindows[i].m_buttons[j].Draw();

            }
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

            uiElement->relics[uiElement->relicIndex].flavorImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * (uiElement->gridPosition.y + 1.25f)) });
            uiElement->relics[uiElement->relicIndex].flavorTitle.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * (uiElement->gridPosition.y + 1.25f)) });
            uiElement->relics[uiElement->relicIndex].flavorText.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * (uiElement->gridPosition.y) + 1.25f) });

            uiElement->relics[uiElement->relicIndex].sprite.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * uiElement->gridPosition.x), spritePixelCoords.y - (0.15f * uiElement->gridPosition.y) });
            uiElement->relicIndex++;
            uiElement->gridPosition.x++;

            RedrawUI();
        } 

        for (uint32_t i = 0; i < uiElement->relics.size(); i++)
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
        auto uiElement = registry.GetComponent<UIShopComponent>(entity);

        uiElement->baseImage.m_UiComponent.SetPosition(uiElement->position);
        uiElement->baseImage.m_UiComponent.SetScale(uiElement->scale);

        ML_String playerInfo = ("Souls: " + std::to_string((int)player->souls) +
            "\n\nPlayer Stats:\nHealth: " + std::to_string((int)stats->health) +
            "\nDamage: " + std::to_string((int)stats->damage) + 
            "\nMove Speed: " + std::to_string((int)stats->moveSpeed) +
            "\nAttack Speed: " + std::to_string((int)stats->attackSpeed)).c_str();

        DirectX::XMFLOAT2 spritePositionOffset = { uiElement->baseImage.m_UiComponent.m_CurrentBounds.right / (uiElement->baseImage.m_UiComponent.m_CurrentBounds.right / 40.0f) ,
                                               uiElement->baseImage.m_UiComponent.m_CurrentBounds.bottom / (uiElement->baseImage.m_UiComponent.m_CurrentBounds.bottom / 40.0f) };

        DirectX::XMFLOAT2 startingSpritePosition = { abs(uiElement->baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
                                       abs(uiElement->baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
        DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
                                        -1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };


        std::wstring valueAsWString(playerInfo.begin(), playerInfo.end());
        uiElement->playerInfo.UpdateText(valueAsWString);
        uiElement->playerInfo.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1), spritePixelCoords.y - (0.1f * 3) });

        uiElement->rerollButton.SetPosition({ spritePixelCoords.x + (0.1f * 7), spritePixelCoords.y - (0.1f * 11) });

        for (uint32_t i = 0; i < uiElement->relicWindows.size(); i++)
        {
            uiElement->relicWindows[i].m_baseImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 7), spritePixelCoords.y - (0.1f * 3 * (i + 1)) });
            uiElement->relicWindows[i].m_relicImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 7.5f), spritePixelCoords.y - (0.1f * 3 * (i + 1)) });

            DirectX::XMFLOAT2 startingSpritePosition2 = { abs(uiElement->relicWindows[i].m_baseImage.m_UiComponent.GetPosition().x + (spritePositionOffset.x / 2.0f)) ,
                                           abs(uiElement->relicWindows[i].m_baseImage.m_UiComponent.GetPosition().y + (spritePositionOffset.y / 2.0f)) };
            DirectX::XMFLOAT2 spritePixelCoords2 = { (startingSpritePosition2.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
                                            -1 * ((startingSpritePosition2.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

            for (uint32_t j = 0; j < uiElement->relicWindows[i].m_buttons.size(); j++)
                uiElement->relicWindows[i].m_buttons[j].SetPosition({ spritePixelCoords2.x + 0.01f, spritePixelCoords2.y - (0.1f * j) });

        }

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

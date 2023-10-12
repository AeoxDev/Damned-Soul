#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Components.h"
#include "SDLHandler.h"

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

            for (uint32_t i = 0; i < uiElement->images.size(); i++)
                uiElement->images[i].Draw();

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


        if (uiElement->imageIndex + 1 == uiElement->images.size())
        {
            float xPos = abs(uiElement->baseImage.m_UiComponent.GetPosition().x - uiElement->baseImage.m_UiComponent.m_CurrentBounds.right / 2);
            float pixelCoordsX = (xPos / 0.5f / sdl.WIDTH) - 1.0f;

            uiElement->images[uiElement->imageIndex].m_UiComponent.SetScale({ 1.0f , 1.0f });
            uiElement->images[uiElement->imageIndex].m_UiComponent.SetPosition({ pixelCoordsX + (0.1f * uiElement->imageIndex), uiElement->position.y /*+ (0.01f * uiElement->imageIndex)*/ });
            uiElement->imageIndex++;
            RedrawUI();
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

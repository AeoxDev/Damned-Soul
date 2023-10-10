#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "UI/UIRenderer.h"
#include "Components.h"

#include <iostream>
#include <iomanip>

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

        for (auto entity : View<UIButtonComponent>(registry))
            registry.GetComponent<UIButtonComponent>(entity)->button.Draw();

        for (auto entity : View<ImageComponent>(registry))
            registry.GetComponent<ImageComponent>(entity)->image.Draw();

        for (auto entity : View<TextComponent>(registry))
            registry.GetComponent<TextComponent>(entity)->text.Draw();

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
        uiElement->value = (float)(stats->health);

        std::string valueAsString = "Health: " + std::to_string((int)uiElement->value);
        std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());

        uiElement->text.UpdateText(valueAsWString);

        uiElement->text.SetScale(uiElement->scale);
        uiElement->text.SetPosition(uiElement->position);

        uiElement->image.SetScale(uiElement->scale);
        uiElement->image.SetPosition(uiElement->position);
    }

    return true;
}

bool UIPlayerSoulsSystem::Update()
{
    for (auto entity : View<UIPlayerSoulsComponent, PlayerComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIPlayerSoulsComponent>(entity);
        auto player = registry.GetComponent<PlayerComponent>(entity);
        uiElement->value = (float)(player->souls);

        std::string valueAsString = "Souls: " + std::to_string((int)uiElement->value);
        std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());

        uiElement->text.UpdateText(valueAsWString);

        uiElement->text.SetScale(uiElement->scale);
        uiElement->text.SetPosition(uiElement->position);

        uiElement->image.SetScale(uiElement->scale);
        uiElement->image.SetPosition(uiElement->position);
    }

    return true;
}


bool UIGameLevelSystem::Update()
{
    for (auto entity : View<UIGameLevelComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIGameLevelComponent>(entity);

        std::string valueAsString = std::to_string(uiElement->value);
        std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());

        uiElement->text.UpdateText(valueAsWString);

        uiElement->text.SetScale(uiElement->scale);
        uiElement->text.SetPosition(uiElement->position);

        uiElement->image.SetScale(uiElement->scale);
        uiElement->image.SetPosition(uiElement->position);
    }
    return true;
}
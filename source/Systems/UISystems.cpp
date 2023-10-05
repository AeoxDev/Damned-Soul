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

        for (auto entity : View<ButtonComponent>(registry))
            registry.GetComponent<ButtonComponent>(entity)->button.Draw();

        for (auto entity : View<ImageComponent>(registry))
            registry.GetComponent<ImageComponent>(entity)->image.Draw();

        for (auto entity : View<TextComponent>(registry))
            registry.GetComponent<TextComponent>(entity)->text.Draw();

        for (auto entity : View<UIPlayerHealthComponent>(registry))
        {
            auto comp = registry.GetComponent<UIPlayerHealthComponent>(entity);
            comp->image.Draw();
            comp->text.Draw();
        }

        for (auto entity : View<UIPlayerSoulsComponent>(registry))
        {
            auto comp = registry.GetComponent<UIPlayerSoulsComponent>(entity);
            comp->image.Draw();
            comp->text.Draw();
        }
        End2dFrame(ui);
    }

    return true;
}

bool PlayerHealthUISystem::Update()
{
    for (auto entity : View<UIPlayerHealthComponent, StatComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIPlayerHealthComponent>(entity);
        auto player = registry.GetComponent<StatComponent>(entity);
        uiElement->value = (float)(player->health);

        std::string valueAsString = "Health: " + std::to_string((int)uiElement->value);

        std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());

        uiElement->text.UpdateText(valueAsWString);

        uiElement->text.SetPosition(uiElement->position);
        uiElement->image.SetPosition(uiElement->position);
    }

    return true;
}

bool PlayerSoulsUISystem::Update()
{
    for (auto entity : View<UIPlayerSoulsComponent, PlayerComponent>(registry))
    {
        auto uiElement = registry.GetComponent<UIPlayerSoulsComponent>(entity);
        auto player = registry.GetComponent<PlayerComponent>(entity);
        uiElement->value = (float)(player->souls);

        std::string valueAsString = "Souls: " + std::to_string((int)uiElement->value);

        std::wstring valueAsWString(valueAsString.begin(), valueAsString.end());

        uiElement->text.UpdateText(valueAsWString);

        uiElement->text.SetPosition(uiElement->position);
        uiElement->image.SetPosition(uiElement->position);
    }

    return true;
}

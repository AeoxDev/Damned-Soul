#include "States_&_Scenes\Settings.h"
#include "States_&_Scenes\StateManager.h"
#include "Input.h"
#include "Menus.h"
#include "UIRenderer.h"
#include "AllComponents.h"


void SettingsState::Setup()
{
	SetupButtons();
	SetupImages();
	SetupText();


	DrawUi();

}

void SettingsState::SetupButtons()
{
	//Back Button
	{
		auto OnClick = [this]()
			{
				SetInMainMenu(true);
				SetInSettings(false);
				stateManager.menu.Setup();
				Unload();
			};

		auto OnHover = [this]()
			{

			};
		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton(ui, "Exmenu/BackButton.png", "BackButton", L"", OnClick, OnHover, { sdl.WIDTH / 2.0f - 65.0f, 800 }));
	}
}


void SettingsState::SetupImages()
{

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/OptionsButtonHover.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/D0.png", { 50.0f, 50.0f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/D0.png", { sdl.WIDTH - 68.0f, 50.0f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage(ui, "ExMenu/CheckBoxBase.png", { sdl.WIDTH / 2.0f + 50.0f, 500.0f }));
}


void SettingsState::SetupText()
{

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"This is the settings menu!", { sdl.WIDTH / 2.0f - 50.0f, 300.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Fullscreen:", { sdl.WIDTH / 2.0f - 50.0f, 500.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Volume: 100", { sdl.WIDTH / 2.0f - 50.0f, 600.0f }));

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(ui, L"Graphics: Flawless", { sdl.WIDTH / 2.0f - 50, 700.0f }));
}

void SettingsState::Clear()
{
}

void SettingsState::ComputeShaders()
{
}

void SettingsState::Render()
{
	RenderUI();
}

void SettingsState::Input()
{

	for (auto entity : View<ButtonComponent>(registry))
	{
		auto comp = registry.GetComponent<ButtonComponent>(entity);
		if (comp->button.m_uiComponent.Intersect({ mouseX, mouseY }))
		{
			comp->button.Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
				comp->button.Interact();
		}
	}
}

void SettingsState::Update()
{
}

void SettingsState::Unload()
{
	for (auto entity : View<ButtonComponent>(registry))
	{
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		registry.DestroyEntity(entity);
	}
}

void SettingsState::DrawUi()
{
	ID2D1RenderTarget* rt = ui.GetRenderTarget();

	UpdateUI2();

	Begin2dFrame(ui);

	for (auto entity : View<ButtonComponent>(registry))
		registry.GetComponent<ButtonComponent>(entity)->button.Draw(ui, rt);

	for (auto entity : View<ImageComponent>(registry))

		registry.GetComponent<ImageComponent>(entity)->image.Draw(rt);

	for (auto entity : View<TextComponent>(registry))
		registry.GetComponent<TextComponent>(entity)->text.Draw(ui);

	End2dFrame(ui);
}
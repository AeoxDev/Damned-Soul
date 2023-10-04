#include "States\Settings.h"
#include "States\StateManager.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UIRenderer.h"

void SettingsState::Setup()
{
	RedrawUI();
	SetupButtons();
	SetupImages();
	SetupText();
}

void SettingsState::Input()
{
	if (keyState[SDL_SCANCODE_0] == pressed)
	{
		
	}
	if (keyState[SDL_SCANCODE_1] == pressed)
	{
		
	}
	if (keyState[SDL_SCANCODE_2] == pressed)
	{
		
	}
}

void SettingsState::Update()
{
}

void SettingsState::ComputeShaders()
{
}

void SettingsState::SetupButtons()
{
	//1280x720
	{
		auto OnClick = [this]()
			{

			};

		auto OnHover = [this]()
			{

			};
		//registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("", "1280x720", L"1280x720", OnClick, OnHover, { 0.0f, 0.0f }, { 2.0f, 2.0f }));
	}

	//1600x900
	{
		auto OnClick = [this]()
			{

			};

		auto OnHover = [this]()
			{

			};
		//registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("", "1600x900", L"1600x900", OnClick, OnHover, { 0.0f, -0.2f }, { 2.0f, 2.0f }));
	}
	
	//1920x1080
	{
		auto OnClick = [this]()
			{

			};

		auto OnHover = [this]()
			{

			};
		//registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("", "1920x1080", L"1920x1080", OnClick, OnHover, { 0.0f, -0.4f }, { 2.0f, 2.0f }));
	}

	//Fullscreen
	{
		auto OnClick = [this]()
			{
			
			};

		auto OnHover = [this]()
			{

			};
		//registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("", "FullscreenButton", L"Fullscreen", OnClick, OnHover, { 0.0f, -0.6f }, { 2.0f, 2.0f }));
	}

	//Back Button
	{
		auto OnClick = [this]()
			{
				SetInMainMenu(true);
				SetInSettings(false);
				Unload();
				stateManager.menu.Setup();
			};

		auto OnHover = [this]()
			{

			};
		registry.AddComponent<ButtonComponent>(registry.CreateEntity(), UIButton("Exmenu/BackButton.png", "BackButton", L"", OnClick, OnHover, { 0.0f, -0.8f }));
	}
}

void SettingsState::SetupImages()
{

	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/OptionsButtonHover.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { -0.5f, 0.6f }, { 1.5f, 1.5f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { 0.5, 0.6f }, { 1.5f, 1.5f }));

/*
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { 0.0f, -0.3f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { -0.3f, 0.0f }, { 2.0f, 1.5f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { -0.0f, 0.0f }, { 2.0f, 1.5f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { 0.3f, 0.0f }, { 2.0f, 1.5f }));
	*/
}

void SettingsState::SetupText()
{

	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Settings", { 0.0f, 0.6f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Fullscreen:", { 0.0f, -0.2f }));

	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Volume: 100", { 0.0f, -0.4f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Graphics: Flawless", { 0.0f, -0.6f }));

	/*
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"1280x720", { -0.3f, 0.0f }));
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"1600x900", { -0.0f, 0.0f }));
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"1920x1080", { 0.3f, 0.0f }));
	*/
}

void SettingsState::Unload()
{
	for (auto entity : View<ButtonComponent>(registry))
	{
		registry.RemoveComponent<ButtonComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		registry.RemoveComponent<ImageComponent>(entity);
		registry.DestroyEntity(entity);
	}

	for (auto entity : View<TextComponent>(registry))
	{
		registry.RemoveComponent<TextComponent>(entity);
		registry.DestroyEntity(entity);
	}
}
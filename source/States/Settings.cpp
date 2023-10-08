#include "States\Settings.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "Camera.h"

void SettingsState::Setup()
{
	m_active = true;

	RedrawUI();
	SetupImages();
	SetupButtons();
	SetupText();

	Camera::ResetCamera();
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
	// Changes resolution to 1280x720
	{
		auto OnClick = [this]()
			{
				SDL_SetWindowSize(sdl.sdlWindow, 1280, 720);
			};

		auto OnHover = [this]()
			{
				
			};
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"1280x720", OnClick, OnHover, { -0.4f, 0.0f }, {0.8f, 0.8f}));
	}

	// Changes resolution to 1600x900
	{
		auto OnClick = [this]()
			{
				SDL_SetWindowSize(sdl.sdlWindow, 1600, 900);
			};

		auto OnHover = [this]()
			{

			};
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"1600x900", OnClick, OnHover, { 0.0f, 0.0f }, { 0.8f, 0.8f }));
	}
	
	// Changes resolution to 1920x1080
	{
		auto OnClick = [this]()
			{
				SDL_SetWindowSize(sdl.sdlWindow, 1920, 1080);
			};

		auto OnHover = [this]()
			{

			};
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"1920x1080", OnClick, OnHover, { 0.4f, 0.0f }, { 0.8f, 0.8f }));
	}

	//Enables/Disables Fullscreen depending on current state
	{
		auto OnClick = [this]()
			{
				Uint32 windowFlags = SDL_GetWindowFlags(sdl.sdlWindow);
				if ((windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				{
					SDL_SetWindowFullscreen(sdl.sdlWindow, 0);
				}
				else
				{
					SDL_SetWindowFullscreen(sdl.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
				}
			};

		auto OnHover = [this]()
			{

			};
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"Fullscreen", OnClick, OnHover, { 0.0f, -0.4f }, { 0.8f, 0.8f }));
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
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"Back", OnClick, OnHover, { -0.8f, -0.8f }, { 0.8f, 0.8f }));
	}
}

void SettingsState::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/ButtonBackgroundHover.png", { 0.0, 0.0f }, { 1.7f, 1.7f }));
/*
	//registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/OptionsButtonHover.png", { (sdl.WIDTH / 2.0f) - (426.0f / 8.0f), 100.0f }, { 1.0f, 1.0f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { -0.5f, 0.6f }, { 1.5f, 1.5f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/D0.png", { 0.5, 0.6f }, { 1.5f, 1.5f }));

	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { 0.0f, -0.3f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { -0.3f, 0.0f }, { 2.0f, 1.5f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { -0.0f, 0.0f }, { 2.0f, 1.5f }));
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/CheckBoxBase.png", { 0.3f, 0.0f }, { 2.0f, 1.5f }));
*/
}

void SettingsState::SetupText()
{
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Settings", { 0.0f, 0.4f }, {1.5f, 1.5f}));

	/*
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Fullscreen:", { 0.0f, -0.2f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Volume: 100", { 0.0f, -0.4f }));
	//registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Graphics: Flawless", { 0.0f, -0.6f }));


	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"1280x720", { -0.3f, 0.0f }));
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"1600x900", { -0.0f, 0.0f }));
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"1920x1080", { 0.3f, 0.0f }));
	*/
}

void SettingsState::Unload()
{
	// If this state is not active, simply skip the unload
	if (false == m_active)
		return;
	m_active = false; // Set active to false

	CREATE_ENTITY_MAP_entities;

	for (auto entity : View<UIButtonComponent>(registry))
	{
		UIButtonComponent* b = registry.GetComponent<UIButtonComponent>(entity);
		b->button.Release();
		registry.RemoveComponent<UIButtonComponent>(entity);

		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<ImageComponent>(registry))
	{
		ImageComponent* i = registry.GetComponent<ImageComponent>(entity);
		i->image.Release();
		registry.RemoveComponent<ImageComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	for (auto entity : View<TextComponent>(registry))
	{
		registry.RemoveComponent<TextComponent>(entity);
		
		ADD_TO_entities_IF_NOT_INCLUDED(entity);
	}

	uint16_t destCount = DestroyEntities(entities);
}
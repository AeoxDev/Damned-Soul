#include "States\Settings.h"
#include "States\StateManager.h"
#include "States\CleanupMacros.h"
#include "Input.h"
#include "Components.h"
#include "Registry.h"
#include "UI/UIRenderer.h"
#include "Camera.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"

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
				if (sdl.windowFlags & SDL_WINDOW_FULLSCREEN)
				{
					SDL_SetWindowFullscreen(sdl.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_GetWindowSizeInPixels(sdl.sdlWindow, (int*)&sdl.WIDTH, (int*)&sdl.HEIGHT);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, sdl.WIDTH, sdl.HEIGHT);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
				}
				else
				{
					sdl.WIDTH = 1280;
					sdl.HEIGHT = 720;
					sdl.WINDOWED_WIDTH = sdl.WIDTH;
					sdl.WINDOWED_HEIGHT = sdl.HEIGHT;
					SDL_SetWindowSize(sdl.sdlWindow, 1280, 720);
					SDL_SetWindowPosition(sdl.sdlWindow, 50, 50);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, 1280, 720);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
				}
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
				if (sdl.windowFlags & SDL_WINDOW_FULLSCREEN)
				{
					SDL_SetWindowFullscreen(sdl.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_GetWindowSizeInPixels(sdl.sdlWindow, (int*)&sdl.WIDTH, (int*)&sdl.HEIGHT);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, sdl.WIDTH, sdl.HEIGHT);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
				}
				else
				{
					sdl.WIDTH = 1600;
					sdl.HEIGHT = 900;
					sdl.WINDOWED_WIDTH = sdl.WIDTH;
					sdl.WINDOWED_HEIGHT = sdl.HEIGHT;
					SDL_SetWindowSize(sdl.sdlWindow, 1600, 900);
					SDL_SetWindowPosition(sdl.sdlWindow, 50, 50);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, 1600, 900);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
				}
				
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
				if (sdl.windowFlags & SDL_WINDOW_FULLSCREEN)
				{
					SDL_SetWindowFullscreen(sdl.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_GetWindowSizeInPixels(sdl.sdlWindow, (int*)&sdl.WIDTH, (int*)&sdl.HEIGHT);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, sdl.WIDTH, sdl.HEIGHT);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
					
				}
				else
				{
					SDL_SetWindowSize(sdl.sdlWindow, 1920, 1080);
					sdl.WIDTH = 1920;
					sdl.HEIGHT = 1080;
					sdl.WINDOWED_WIDTH = sdl.WIDTH;
					sdl.WINDOWED_HEIGHT = sdl.HEIGHT;
					EditViewport(renderStates[backBufferRenderSlot].viewPort, sdl.WIDTH, sdl.HEIGHT);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
				}
				
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
				sdl.windowFlags = SDL_GetWindowFlags(sdl.sdlWindow);
				if ((sdl.windowFlags & SDL_WINDOW_FULLSCREEN_DESKTOP) != 0)
				{
					SDL_SetWindowFullscreen(sdl.sdlWindow, 0);
					sdl.windowFlags = sdl.windowFlags & (~SDL_WINDOW_FULLSCREEN_DESKTOP);
					sdl.WIDTH = sdl.WINDOWED_WIDTH;
					sdl.HEIGHT = sdl.WINDOWED_HEIGHT;
					SDL_SetWindowSize(sdl.sdlWindow, sdl.WIDTH, sdl.HEIGHT);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, sdl.WIDTH, sdl.HEIGHT);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
					SDL_SetWindowPosition(sdl.sdlWindow, 50, 50);
				}
				else
				{
					sdl.windowFlags = sdl.windowFlags | SDL_WINDOW_FULLSCREEN_DESKTOP;
					SDL_SetWindowFullscreen(sdl.sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
					SDL_GetWindowSizeInPixels(sdl.sdlWindow, (int*)&sdl.WIDTH, (int*)&sdl.HEIGHT);
					EditViewport(renderStates[backBufferRenderSlot].viewPort, sdl.WIDTH, sdl.HEIGHT);
					SetViewport(renderStates[backBufferRenderSlot].viewPort);
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
		registry.AddComponent<UIButtonComponent>(registry.CreateEntity(), UIButton("ExMenu/ButtonBackground.png", "ExMenu/ButtonBackgroundHover.png", L"Back", OnClick, OnHover, { -0.6f, 0.0f }, { 0.8f, 0.8f }));
	}
}

void SettingsState::SetupImages()
{
	registry.AddComponent<ImageComponent>(registry.CreateEntity(), UIImage("ExMenu/ButtonBackgroundHover.png", { 0.0, 0.0f }, { 1.7f, 1.7f }));
}

void SettingsState::SetupText()
{
	registry.AddComponent<TextComponent>(registry.CreateEntity(), UIText(L"Settings", { 0.0f, 0.4f }, {1.5f, 1.5f}));
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
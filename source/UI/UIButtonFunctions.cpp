#include "UIButtonFunctions.h"
#include "States\StateManager.h"
#include "SDLHandler.h"
#include "Level.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"

#include "Registry.h"
#include "Components.h"
#include "RelicFunctions.h"
#include "UIRenderer.h"


void UIFunc::LoadNextLevel(void* args)
{
	SetInPlay(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	LoadLevel(++stateManager.activeLevel);
}

void UIFunc::MainMenu_Settings(void* args)
{
	SetInSettings(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	stateManager.settings.Setup();
}

void UIFunc::MainMenu_Quit(void* args)
{
	sdl.quit = true;
}


void UIFunc::Settings_Back(void* args)
{
	SetInMainMenu(true);
	SetInSettings(false);
	stateManager.settings.Unload();
	stateManager.menu.Setup();
}

void UIFunc::Settings_LowRes(void* args)
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
}

void UIFunc::Settings_MediumRes(void* args)
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
}

void UIFunc::Settings_HighRes(void* args)
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
}

void UIFunc::Settings_Fullscreen(void* args)
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
}

void UIFunc::Shop_BuyRelic(void* args)
{
	int a = 0;
}

void UIFunc::Shop_LockRelic(void* args)
{

	int a = 0;
}
//Creates memory leaks, cant fix without mattias help with UIimage
void UIFunc::Shop_ReRollRelic(void* args)
{

	ML_Vector<Relics::RelicMetaData> relics;

	for (int i = 0; i < 6; i++)
	{
		switch (i)
		{
		case 0: relics.push_back(Relics::DemonBonemarrow(false)); break;
		case 1: relics.push_back(Relics::FlameWeapon(false)); break;
		case 2: relics.push_back(Relics::SoulPower(false)); break;
		case 3: relics.push_back(Relics::DemonHeart(false)); break;
		case 4: relics.push_back(Relics::FrostFire(false)); break;
		case 5: relics.push_back(Relics::SoulHealth(false)); break;
		default:
			break;
		}

	}

	for (auto entity : View<UIShopRelicWindowComponent>(registry))
	{
		auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);

		DirectX::XMFLOAT2 spritePositionOffset = { uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.right / (uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
											  uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.bottom / (uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

		DirectX::XMFLOAT2 startingSpritePosition = { abs(uiElement->m_baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
									   abs(uiElement->m_baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
		DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
										-1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

		int rnd = rand() % 6;
		uiElement->m_relicImage.Setup(relics[rnd].filePath);
		uiElement->m_relicImage.m_UiComponent.SetScale({ 1.5f, 1.5f });
		uiElement->m_relicImage.m_UiComponent.SetPosition({ spritePixelCoords.x + (0.1f * 1.5f), spritePixelCoords.y - (0.05f * 1) });

	}
	RedrawUI();
}

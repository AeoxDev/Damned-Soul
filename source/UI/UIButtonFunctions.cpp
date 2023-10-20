#include "UIButtonFunctions.h"
#include "States\StateManager.h"
#include "SDLHandler.h"
#include "Level.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"

#include "Registry.h"
#include "Components.h"
#include "UIRenderer.h"

#include <random>

void UIFunc::LoadNextLevel(void* args)
{
	SetInPlay(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	//LoadLevel(++stateManager.activeLevel);
	LoadLevel(2);
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
	UIButton* button = (UIButton*)args;

	int counter = 1;
	for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	{
		if (counter != button->shopPosition)
		{
			counter++;
			continue;
		}

		switch (registry.GetComponent<UIRelicComponent>(entity)->relicIndex)
		{
		case DemonBonemarrow:
			Relics::DemonBonemarrow(true);
			break;
		case FlameWeapon:
			Relics::FlameWeapon(true);
			break;
		case SoulPower:
			Relics::SoulPower(true);
			break;
		case DemonHeart:
			Relics::DemonHeart(true);
			break;
		case FrostFire:
			Relics::FrostFire(true);
			break;
		case SoulHealth:
			Relics::SoulHealth(true);
			break;
		default:
			break;
		}

		RedrawUI();
		break;
	}
}


void UIFunc::Shop_LockRelic(void* args)
{
	UIButton* button = (UIButton*)args;

	int counter = 1;
	for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	{
		if (counter != button->shopPosition)
		{
			counter++;
			continue;
		}
		auto ui = registry.GetComponent<UIRelicComponent>(entity);
		ui->locked *= -1;

		RedrawUI();
		break;
	}
}

void UIFunc::Shop_ReRollRelic(void* args)
{
	std::random_device dev;
	std::mt19937 rng(dev());
	std::uniform_int_distribution<std::mt19937::result_type> randomNumber(0, 512);
	int rnd = -1;
	ML_Array<int, 3> includedRelics;
	ML_Vector<int> allRelics;

	for (int i = 0; i < 6; i++)
		allRelics.push_back(i);

	for (int i = 0; i < 3; i++)
	{
		rnd = randomNumber(rng) % allRelics.size();
		includedRelics[i] = allRelics[rnd];
		allRelics.erase(rnd);
	}

	int counter = 0;
	for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	{
		auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
		auto uiRelic = registry.GetComponent<UIRelicComponent>(entity);

		DirectX::XMFLOAT2 spritePositionOffset = { uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.right / (uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.right / 32.0f) ,
											  uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.bottom / (uiElement->m_baseImage.m_UiComponent.m_CurrentBounds.bottom / 32.0f) };

		DirectX::XMFLOAT2 startingSpritePosition = { abs(uiElement->m_baseImage.m_UiComponent.GetPosition().x + spritePositionOffset.x) ,
									   abs(uiElement->m_baseImage.m_UiComponent.GetPosition().y + spritePositionOffset.y) };
		DirectX::XMFLOAT2 spritePixelCoords = { (startingSpritePosition.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
										-1 * ((startingSpritePosition.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };
		
		if (uiRelic->locked == 1)
		{
			counter++;
			continue;
		}

		uiRelic->relicIndex = includedRelics[counter];
		switch (includedRelics[counter])
		{
		case DemonBonemarrow:
			uiRelic->sprite.SetImage(Relics::DemonBonemarrow(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::DemonBonemarrow(false).relicName, true);
			uiRelic->flavorText.UpdateText(Relics::DemonBonemarrow(false).description, true);
			break;
		case FlameWeapon:
			uiRelic->sprite.SetImage(Relics::FlameWeapon(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::FlameWeapon(false).relicName, true);
			uiRelic->flavorText.UpdateText(Relics::FlameWeapon(false).description, true);
			break;
		case SoulPower:
			uiRelic->sprite.SetImage(Relics::SoulPower(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::SoulPower(false).relicName, true);
			uiRelic->flavorText.UpdateText(Relics::SoulPower(false).description, true);
			break;
		case DemonHeart:
			uiRelic->sprite.SetImage(Relics::DemonHeart(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::DemonHeart(false).relicName, true);
			uiRelic->flavorText.UpdateText(Relics::DemonHeart(false).description, true);
			break;
		case FrostFire:
			uiRelic->sprite.SetImage(Relics::FrostFire(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::FrostFire(false).relicName, true);
			uiRelic->flavorText.UpdateText(Relics::FrostFire(false).description, true);
			break;
		case SoulHealth:
			uiRelic->sprite.SetImage(Relics::SoulHealth(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::SoulHealth(false).relicName, true);
			uiRelic->flavorText.UpdateText(Relics::SoulHealth(false).description, true);
			break;
		default:
			break;
		}

		counter++;
	}

	RedrawUI();
}
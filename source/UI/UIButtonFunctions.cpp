#include "UIButtonFunctions.h"
#include "States\StateManager.h"
#include "SDLHandler.h"
#include "Level.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "Registry.h"
#include "Components.h"

#include "Registry.h"
#include "Components.h"
#include "UIRenderer.h"
#include "UIComponents.h"

#include <random>

void UIFunc::LoadNextLevel(void* args)
{
	SetInPlay(true);
	SetInMainMenu(false);
	UnloadEntities();
	for (auto entity : View<AudioEngineComponent>(registry))
	{
		SoundComponent* backgroundMusic = registry.GetComponent<SoundComponent>(entity);
		backgroundMusic->Stop();
		AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
		audioJungle->HandleSound();
		backgroundMusic->Play(Music_StageCalm, Channel_Base);
		//audioJungle->HandleSound();
		//backgroundMusic->Play(Music_StageCombat, Channel_Extra); Add back when music for combat is good and can fade from one to another.
	}

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
	UnloadEntities();
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
		SDL_SetWindowPosition(sdl.sdlWindow, 0, 25);
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
		SDL_SetWindowPosition(sdl.sdlWindow, 0, 25);
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
	PlayerComponent* player = nullptr;

	for (auto entity : View<PlayerComponent, StatComponent>(registry))
		player = registry.GetComponent<PlayerComponent>(entity);

	for (auto entity : View<UIButton>(registry))
	{
		const UIButton* other = registry.GetComponent<UIButton>(entity);
		if (button != other)
			continue;
		
		for (auto uiEntity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
		{
			auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(uiEntity);
			auto uiRelic = registry.GetComponent<UIRelicComponent>(uiEntity);

			if (uiElement->shopPosition != button->shopPosition)
				continue;

			if (player->GetSouls() < uiRelic->price /*|| uiElement->bought*/)
				break;

			/*uiElement->bought = true;

			button->m_Images[0].SetImage("Dollar.png");*/

			player->UpdateSouls(-uiRelic->price);
			switch (uiRelic->relicIndex)
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
			case SpeedyLittleDevil:
				Relics::SpeedyLittleDevil(true);
				break;
			case LightningGod:
				Relics::LightningGod(true);
				break;
			default:
				break;
			}

			RedrawUI();
			break;
		}		
	}
}


void UIFunc::Shop_LockRelic(void* args)
{
	UIButton* button = (UIButton*)args;

	for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	{
		auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
		auto uiRelic = registry.GetComponent<UIRelicComponent>(entity);
		
		if (uiElement->shopPosition != button->shopPosition)
			continue;
		
		uiRelic->locked *= -1;

		if (uiRelic->locked == -1)
		{
			button->m_Images[0].SetImage("Unlocked.png");
			button->m_Images[1].SetImage("Unlocked.png");
		}
		else
		{
			button->m_Images[0].SetImage("Locked.png");
			button->m_Images[1].SetImage("Locked.png");
		}

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

	for (int i = 0; i < 8; i++)
		allRelics.push_back(i);

	for (int i = 0; i < 3; i++)
	{
		rnd = randomNumber(rng) % allRelics.size();
		includedRelics[i] = allRelics[rnd];
		allRelics.erase(rnd);
	}

	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);

	int counter = 0;
	for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	{
		auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
		auto uiRelic = registry.GetComponent<UIRelicComponent>(entity);

		if (player->GetSouls() < 1)
			break;

		if (uiRelic->locked == 1)
		{
			counter++;
			continue;
		}

		/*if (uiElement->bought && args)
		{
			for (auto buttonEntity : View<UIButton>(registry))
			{
				auto button = registry.GetComponent<UIButton>(buttonEntity);
				if (uiElement->shopPosition != button->shopPosition)
					continue;

				button->m_Images[0].SetImage("TempBuy.png");
				break;
			}

			continue;
		}*/
		

		uiRelic->relicIndex = includedRelics[counter];
		switch (includedRelics[counter])
		{
		case DemonBonemarrow:
			uiRelic->price = 10;
			uiRelic->sprite.SetImage(Relics::DemonBonemarrow(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::DemonBonemarrow(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::DemonBonemarrow(false).description, true);
			break;
		case FlameWeapon:
			uiRelic->price = 3;
			uiRelic->sprite.SetImage(Relics::FlameWeapon(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::FlameWeapon(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::FlameWeapon(false).description, true);
			break;
		case SoulPower:
			uiRelic->price = 5;
			uiRelic->sprite.SetImage(Relics::SoulPower(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::SoulPower(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::SoulPower(false).description, true);
			break;
		case DemonHeart:
			uiRelic->price = 10;
			uiRelic->sprite.SetImage(Relics::DemonHeart(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::DemonHeart(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::DemonHeart(false).description, true);
			break;
		case FrostFire:
			uiRelic->price = 3;
			uiRelic->sprite.SetImage(Relics::FrostFire(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::FrostFire(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::FrostFire(false).description, true);
			break;
		case SoulHealth:
			uiRelic->price = 5;
			uiRelic->sprite.SetImage(Relics::SoulHealth(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::SoulHealth(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::SoulHealth(false).description, true);
			break;
		case SpeedyLittleDevil:
			uiRelic->price = 3;
			uiRelic->sprite.SetImage(Relics::SpeedyLittleDevil(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::SpeedyLittleDevil(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::SpeedyLittleDevil(false).description, true);
			break;
		case LightningGod:
			uiRelic->price = 10;
			uiRelic->sprite.SetImage(Relics::LightningGod(false).filePath);
			uiRelic->flavorTitle.UpdateText(Relics::LightningGod(false).relicName, true);
			uiRelic->flavorDesc.UpdateText(Relics::LightningGod(false).description, true);
			break;
		default:
			break;
		}

		counter++;
	}

	if (args && player->GetSouls() >= 1)
		player->UpdateSouls(-1);

	RedrawUI();
}

void UIFunc::Shop_Heal(void* args)
{
	for (auto entity : View<PlayerComponent, StatComponent>(registry))
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		StatComponent* stats = registry.GetComponent<StatComponent>(entity);

		if (player->GetSouls() < 2)
			break;

		if (stats->GetHealth() == stats->GetMaxHealth())
			break;

		float heal = stats->GetMaxHealth() * 0.25f;

		stats->UpdateHealth(heal);

		player->UpdateSouls(-2);
	}
}

#include "UIButtonFunctions.h"
#include "States\StateManager.h"
#include "SDLHandler.h"
#include "Level.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "Registry.h"
#include "Components.h"
#include "DeltaTime.h"
#include "Registry.h"
#include "Components.h"
#include "UIRenderer.h"
#include "UIComponents.h"
#include "Input.h"

#include <random>

void UIFunc::LoadNextLevel(void* args, int a)
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
	//LoadLevel(2);
}

void UIFunc::MainMenu_Settings(void* args, int a)
{
	SetInSettings(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	stateManager.settings.Setup();
}

void UIFunc::MainMenu_Quit(void* args, int a)
{
	UnloadEntities();
	sdl.quit = true;
}

void UIFunc::PauseState_ResumeGame(void* args, int a)
{
	if (currentStates & State::InShop)
	{
		SetInPause(false);
		SetInShop(true);
		RedrawUI();
		gameSpeed = 1.0f;
		ResetInput();

		UnloadEntities(ENT_PERSIST_PAUSE);
	}
	else
	{
		SetInPause(false);
		SetInPlay(true);
		RedrawUI();
		gameSpeed = 1.0f;
		ResetInput();

		UnloadEntities(ENT_PERSIST_PAUSE);
	}
}

void UIFunc::PauseState_MainMenu(void* args, int a)
{
	SetInMainMenu(true);
	SetInPause(false);
	UnloadEntities(ENT_PERSIST_LEVEL);
	gameSpeed = 1.0f;
	stateManager.menu.Setup();
}

void UIFunc::Settings_Back(void* args, int a)
{
	SetInMainMenu(true);
	SetInSettings(false);
	stateManager.settings.Unload();
	stateManager.menu.Setup();
}

void UIFunc::Settings_LowRes(void* args, int a)
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

void UIFunc::Settings_MediumRes(void* args, int a)
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

void UIFunc::Settings_HighRes(void* args, int a)
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

void UIFunc::Settings_Fullscreen(void* args, int a)
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

void UIFunc::Shop_BuyRelic(void* args, int a)
{
	//UIButton* button = (UIButton*)args;
	//PlayerComponent* player = nullptr;

	//for (auto entity : View<PlayerComponent, StatComponent>(registry))
	//	player = registry.GetComponent<PlayerComponent>(entity);

	//for (auto entity : View<UIButton>(registry))
	//{
	//	const UIButton* other = registry.GetComponent<UIButton>(entity);
	//	if (button != other)
	//		continue;
	//	
	//	for (auto uiEntity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	//	{
	//		auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(uiEntity);
	//		auto uiRelic = registry.GetComponent<UIRelicComponent>(uiEntity);

	//		if (uiElement->shopPosition != button->shopPosition)
	//			continue;

	//		if (player->GetSouls() < uiRelic->price /*|| uiElement->bought*/)
	//			break;

	//		/*uiElement->bought = true;

	//		button->m_Images[0].SetImage("Dollar.png");*/

			//button->m_Images[0].SetImage("Dollar.png");

	//		player->UpdateSouls(-uiRelic->price);
	//		uiRelic->relicData->m_function(&(stateManager.player));
}

void UIFunc::Shop_LockRelic(void* args)
{
	/*UIButton* button = (UIButton*)args;

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
	}*/
}

void UIFunc::Shop_ReRollRelic(void* args)
{

	//PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);

	//// Put all relics in the shop back, if they aren't bought, locked, or nonexistent
	//for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	//{
	//	auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
	//	auto uiRelic = registry.GetComponent<UIRelicComponent>(entity);

	//	if (uiRelic->locked == 1)
	//	{
	//		uiElement->bought = false;
	//		continue;
	//	}

	//	// If the relic is not bought and the relic has a value, put it back
	//	if (false == uiElement->bought)
	//		Relics::PutBackRelic(uiRelic->relicData);
	//}

	////int counter = 0;
	//// Put in new relics into the shop
	//for (auto entity : View<UIShopRelicWindowComponent, UIRelicComponent>(registry))
	//{
	//	auto uiElement = registry.GetComponent<UIShopRelicWindowComponent>(entity);
	//	auto uiRelic = registry.GetComponent<UIRelicComponent>(entity);

	////	if (player->GetSouls() < 1)
	////		break;

	//	if (uiRelic->locked == 1)
	//	{
	//		//counter++;
	//		uiElement->bought = false;
	//		continue;
	//	}

	//	// Very iffy code to circumvent const here, but we want const everywhere to prevent people from modifying the data itself
	//	// This is not pretty, but its the only thing we know "works" at the moment
	//	const RelicData* selected = Relics::PickRandomRelic(Relics::RELIC_UNTYPED);
	//	std::memcpy(&uiRelic->relicData, &selected, sizeof(RelicData*));

	//	uiRelic->price = uiRelic->relicData->m_price;
	//	uiRelic->sprite.SetImage(uiRelic->relicData->m_filePath);
	//	uiRelic->flavorTitle.UpdateText(uiRelic->relicData->m_relicName, true);
	//	uiRelic->flavorDesc.UpdateText(uiRelic->relicData->m_description, true);
	//	uiElement->bought = false;
	//	//counter++;
	//}

	//if (args && player->GetSouls() >= 1)
	//	player->UpdateSouls(-1);

	//RedrawUI();
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

void UIFunc::HoverImage(void* args, int index, bool hover)
{
	UIComponent* ui = (UIComponent*)args;
	ML_String fileName = "";
	ML_String hoverFileName = "";
	if (ui->m_Images.size() == 0)
	{
		fileName = ui->m_BaseImage.m_fileName;
		hoverFileName = fileName;
		hoverFileName.append("Hover");

		if (hover)
			ui->m_BaseImage.SetImage(hoverFileName.c_str(), true);
		else
			ui->m_BaseImage.SetImage(fileName.c_str(), true);
	}
	else
	{
		fileName = ui->m_Images[index].m_fileName;
		hoverFileName = fileName;
		hoverFileName.append("Hover");

		if (hover)
			ui->m_Images[index].SetImage(hoverFileName.c_str(), true);
		else
			ui->m_Images[index].SetImage(fileName.c_str(), true);
	}
}

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

#define SIZEOF_ARR(arr) (sizeof(arr)/sizeof(arr[0]))

void UIFunc::LoadNextLevel(void* args, int a)
{
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

void UIFunc::SelectRelic(void* args, int index)
{
	UIComponent* uiElement = (UIComponent*)args;

	int inverseIndex = 0;
	int imageIndexes[2];

	if (index == 1)
	{
		inverseIndex = 1;
		imageIndexes[0] = uiElement->m_Images.size() - 2;
		imageIndexes[1] = uiElement->m_Images.size() - 1;
	}
	else
	{
		inverseIndex = 0;
		imageIndexes[0] = uiElement->m_Images.size() - 1;
		imageIndexes[1] = uiElement->m_Images.size() - 2;
	}

	for (auto entity : View<UIRelicWindowComponent>(registry))
	{
		UIComponent* otherUI = registry.GetComponent<UIComponent>(entity);

		if (uiElement != otherUI)
		{
			UIRelicWindowComponent* otherWindow = registry.GetComponent<UIRelicWindowComponent>(entity);
			for (uint32_t i = 0; i < SIZEOF_ARR(otherWindow->shopSelections); i++)
			{
				if (otherWindow->shopSelections[i] == shopState::SELECTED)
				{
					otherWindow->shopSelections[i] = shopState::AVALIABLE;

					otherUI->m_Images[otherUI->m_Images.size() - (2 - i)].SetImage("RelicIcons\\HoverRelic");
					otherUI->m_Images[otherUI->m_Images.size() - (2 - i)].baseUI.SetVisibility(false);
				}
			}

			continue;
		}

		UIRelicWindowComponent* uiWindow = registry.GetComponent<UIRelicWindowComponent>(entity);

		if (uiWindow->shopSelections[index - 1] != shopState::LOCKED && uiWindow->shopSelections[index - 1] != shopState::BOUGHT)
		{
			uiElement->m_Images[imageIndexes[0]].SetImage("RelicIcons\\SelectedRelic");
			uiElement->m_Images[imageIndexes[0]].baseUI.SetVisibility(true);
		}

		if (uiWindow->shopSelections[inverseIndex] != shopState::LOCKED && uiWindow->shopSelections[inverseIndex] != shopState::BOUGHT)
		{
			uiElement->m_Images[imageIndexes[1]].SetImage("RelicIcons\\HoverRelic");
			uiElement->m_Images[imageIndexes[1]].baseUI.SetVisibility(false);
			uiWindow->shopSelections[inverseIndex] = shopState::AVALIABLE;
		}

		if (uiWindow->shopSelections[index - 1] == shopState::AVALIABLE)
		{
			uiWindow->shopSelections[index - 1] = shopState::SELECTED;
		}
		else if (uiWindow->shopSelections[index - 1] == shopState::SELECTED)
		{
			uiElement->m_Images[imageIndexes[0]].SetImage("RelicIcons\\HoverRelic");
			uiWindow->shopSelections[index - 1] = shopState::AVALIABLE;
		}
	}

	RedrawUI();
}

void UIFunc::BuyRelic(void* args, int index)
{
	PlayerComponent* player = nullptr;

	for (auto entity : View<PlayerComponent>(registry))
		player = registry.GetComponent<PlayerComponent>(entity);

	for (auto entity : View<UIRelicWindowComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UIRelicWindowComponent* relicWindow = registry.GetComponent<UIRelicWindowComponent>(entity);

		for (int i = 0; i < 2; i++)
		{
			if (relicWindow->shopSelections[i] == shopState::SELECTED)
			{
				relicWindow->shopSelections[i] = shopState::BOUGHT;
				
				uiElement->m_Images[i + 2].SetImage("Buy");
				relicWindow->shopRelics[i]->m_function(&stateManager.player);

				player->UpdateSouls(-relicWindow->shopRelics[i]->m_price);
				break;
			}
		}
	}

	RedrawUI();
}

void UIFunc::LockRelic(void* args, int index)
{
	PlayerComponent* player = nullptr;

	for (auto entity : View<PlayerComponent>(registry))
		player = registry.GetComponent<PlayerComponent>(entity);

	if (player->GetSouls() > 0)
	{
		for (auto entity : View<UIRelicWindowComponent>(registry))
		{
			UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
			UIRelicWindowComponent* relicWindow = registry.GetComponent<UIRelicWindowComponent>(entity);

			for (int i = 0; i < 2; i++)
			{
				if (relicWindow->shopSelections[i] == shopState::SELECTED)
				{
					relicWindow->shopSelections[i] = shopState::LOCKED;
					uiElement->m_Images[i + 2].SetImage("RelicIcons\\LockedRelic");
					player->UpdateSouls(0);
					break;
				}
			}
		}
		
	}

	RedrawUI();
}

void UIFunc::RerollRelic(void* args, int index)
{
	UIComponent* uiElement = (UIComponent*)args;
	UIRerollComponent* uiReroll = nullptr;

	PlayerComponent* player = nullptr;

	for (auto entity : View<PlayerComponent>(registry))
		player = registry.GetComponent<PlayerComponent>(entity);

	for (auto entity : View<UIRerollComponent>(registry))
		uiReroll = registry.GetComponent<UIRerollComponent>(entity);

	if (index == -1)
		uiReroll->locked = false;

	if (!uiReroll->locked)
	{
		if (player->GetSouls() > 0)
		{
			for (auto entity : View<UIRelicWindowComponent>(registry))
			{
				UIComponent* uiRelic = registry.GetComponent<UIComponent>(entity);
				UIRelicWindowComponent* relicWindow = registry.GetComponent<UIRelicWindowComponent>(entity);

				for (int i = 0; i < 2; i++)
				{
					bool ignore = false;
					if (relicWindow->shopSelections[i] == shopState::BOUGHT)
					{
						if (index != -1)
							continue;
						
						relicWindow->shopSelections[i] = shopState::AVALIABLE;
						uiRelic->m_Images[i + 2].SetImage("RelicIcons\\HoverRelic");
						uiRelic->m_Images[i + 2].baseUI.SetVisibility(false);
						ignore = true;
					}

					if (relicWindow->shopSelections[i] == shopState::LOCKED || relicWindow->shopSelections[i] == shopState::SELECTED)
					{
						relicWindow->shopSelections[i] = shopState::AVALIABLE;
						uiRelic->m_Images[i + 2].SetImage("RelicIcons\\HoverRelic");
						uiRelic->m_Images[i + 2].baseUI.SetVisibility(false);

						continue;
					}

					if (!ignore)
					{
						relicWindow->shopSelections[i] = shopState::AVALIABLE;
						Relics::PutBackRelic(relicWindow->shopRelics[i]);
					}
					
					const RelicData* relic = Relics::PickRandomRelic(Relics::RELIC_UNTYPED);
					uiRelic->m_Images[i].SetImage(relic->m_filePath);
					relicWindow->shopRelics[i] = relic;
				}
			}

			player->UpdateSouls(0);

			if (index != -1)
				uiReroll->locked = true;
		}
	}

	RedrawUI();
}

void UIFunc::EmptyOnClick(void* args, int index)
{

}

void UIFunc::EmptyOnHover(void* args, int index, bool hover)
{

}

void UIFunc::HealPlayer(void* args, int index)
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

		stats->ApplyHealing(heal);

		player->UpdateSouls(-2);
	}
}

void UIFunc::HoverImage(void* args, int index, bool hover)
{
	UIComponent* uiElement = (UIComponent*)args;
	char fileName[RELIC_DATA_PATH_SIZE] = "";
	char hoverFileName[RELIC_DATA_PATH_SIZE] = "";
	if (uiElement->m_Images.size() == 0)
	{
		if (uiElement->m_BaseImage.baseUI.GetVisibility())
		{
			std::strcpy(fileName, uiElement->m_BaseImage.m_fileName.c_str());
			std::strcpy(hoverFileName, fileName);
			std::strcpy(hoverFileName + std::strlen(hoverFileName), "Hover");
			//hoverFileName.append("Hover");

			if (hover)
				uiElement->m_BaseImage.SetImage(hoverFileName/*.c_str()*/, true);
			else
				uiElement->m_BaseImage.SetImage(fileName/*.c_str()*/, true);
		}
	}
	else if (uiElement->m_Images[index].baseUI.GetVisibility())
	{
		std::strcpy(fileName, uiElement->m_Images[index].m_fileName.c_str());
		std::strcpy(hoverFileName, fileName);
		std::strcpy(hoverFileName + std::strlen(hoverFileName), "Hover");

		if (hover)
			uiElement->m_Images[index].SetImage(hoverFileName, true);
		else
			uiElement->m_Images[index].SetImage(fileName, true);
		
	}
}

void UIFunc::HoverRelic(void* args, int index, bool hover)
{
	UIComponent* uiElement = (UIComponent*)args;
	UIRelicWindowComponent* relicWindow = nullptr;

	if (!uiElement->m_BaseImage.baseUI.GetVisibility())
		return;

	UIComponent* uiImpElement = nullptr;
	UIShopImpComponent* uiImpText = nullptr;

	for (auto entity : View<UIRelicWindowComponent>(registry))
	{
		UIComponent* otherUI = registry.GetComponent<UIComponent>(entity);

		if (uiElement != otherUI)
			continue;

		relicWindow = registry.GetComponent<UIRelicWindowComponent>(entity);
		
	}

	for (auto entity : View<UIShopImpComponent>(registry))
	{
		uiImpElement = registry.GetComponent<UIComponent>(entity);
		uiImpText = registry.GetComponent<UIShopImpComponent>(entity);
	}

	int imageIndex = uiElement->m_Images.size() - (2 - index + 1);

	if (hover)
	{
		uiElement->m_Images[imageIndex].baseUI.SetVisibility(true);
		uiElement->m_Images[imageIndex].baseUI.SetPosition(uiElement->m_Images[index - 1].baseUI.GetPosition());

		uiImpText->name = relicWindow->shopRelics[index - 1]->m_relicName;
		uiImpText->description = relicWindow->shopRelics[index - 1]->m_description;
		uiImpText->price = relicWindow->shopRelics[index - 1]->m_price;

		char relicText[RELIC_DATA_DESC_SIZE] = "";

		std::strcpy(relicText, uiImpText->name.c_str());
		std::strcpy(relicText + std::strlen(relicText), "\nPrice: ");
		std::strcpy(relicText + std::strlen(relicText), (std::to_string(uiImpText->price) + " Souls\n").c_str());
		std::strcpy(relicText + std::strlen(relicText), uiImpText->description.c_str());

		//ML_String relicText = uiImpText->name;
		//relicText.append("\nPrice: ");
		//relicText.append((std::to_string(uiImpText->price) + " Souls\n").c_str());
		//relicText.append(uiImpText->description);

		uiImpElement->m_Text.SetText(relicText, uiImpElement->m_Text.baseUI.GetBounds());
	}
	else
	{
		if (relicWindow->shopSelections[index - 1] == shopState::AVALIABLE)
		{
			uiElement->m_Images[imageIndex].baseUI.SetVisibility(false);
		}
	}

}

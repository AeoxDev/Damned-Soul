#include "UIButtonFunctions.h"
#include "States\StateManager.h"
#include "SDLHandler.h"
#include "Level.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Registry.h"
#include "Components.h"
#include "UIRenderer.h"
#include "UIComponents.h"
#include "Input.h"
#include "Camera.h"
#include "MemLib/ML_Map.hpp"
#include "Relics/Utility/RelicFuncInputTypes.h"

#include "EventFunctions.h"
#include "Levels/LevelHelper.h" //Move CreatePlayer to the Start-button instead of being hardcoded to Level1.cpp

#include <random>

	
void UIFunctions::MainMenu::Start(void* args, int a)
{
	UnloadEntities(ENT_PERSIST_LEVEL);
	for (auto entity : View<AudioEngineComponent>(registry))
	{
		SoundComponent* backgroundMusic = registry.GetComponent<SoundComponent>(entity);
		backgroundMusic->Stop(Channel_Base);
		backgroundMusic->Stop(Channel_Extra);
		AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
		audioJungle->HandleSound();
		backgroundMusic->Play(Music_Hot, Channel_Base);
		backgroundMusic->Play(Ambience_Cave, Channel_Extra);
		audioJungle->HandleSound();
	}


	//Create player when we start the game instead, rather than specifically when Level1 starts (reason: debug later levels without having to run through everything)
	//Niclas was here :)
	CreatePlayer(-0.0f, 0.0f, -0.0f, 80.0f, 100.0f, 20.0f, 10.0f, 1.0f, 1, 0.0f, 0.0, -1.0f);

	stateManager.activeLevel = 0; //Level actually being loaded: activeLevel / 2 + 1
	LoadLevel(++stateManager.activeLevel);

	SetPaused(false);
}

void UIFunctions::MainMenu::SetSettings(void* args, int a)
{
	SetInSettings(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	stateManager.settings.Setup();
}

void UIFunctions::MainMenu::SetCredits(void* args, int a)
{
	SetInCredits(true);
	SetInMainMenu(false);
	stateManager.menu.Unload();
	stateManager.credits.Setup();
}

void UIFunctions::MainMenu::Quit(void* args, int a)
{
	UnloadEntities(ENT_PERSIST_HIGHEST);
	sdl.quit = true;
	stateManager.systems.clear();
}


void UIFunctions::Game::LoadNextLevel(void* args, int a)
{
	UnloadEntities();

	for (auto entity : View<AudioEngineComponent>(registry))
	{
		SoundComponent* backgroundMusic = registry.GetComponent<SoundComponent>(entity);
		backgroundMusic->Stop(Channel_Base);
		backgroundMusic->Stop(Channel_Extra);
		AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
		audioJungle->HandleSound();

		switch (stateManager.activeLevel)
		{
		case 2: //To stage 2
			backgroundMusic->Play(Music_Hot, Channel_Base);
			backgroundMusic->Play(Ambience_Cave, Channel_Extra);
			break;
		case 4: //To stage 3
			backgroundMusic->Play(Music_Hot, Channel_Base);
			backgroundMusic->Play(Ambience_Cave, Channel_Extra);
			break;
		case 6: //To stage 4
		{
			SoundComponent* sfx = registry.GetComponent<SoundComponent>(stateManager.player); //Move this to after boss cutscene is done
			if (sfx != nullptr)
			{
				int soundToPlay = rand() % 2;
				switch (soundToPlay) //Play player boss encounter sound
				{
				case 0:
					sfx->Play(Player_BringItOn, Channel_Extra);
					break;
				case 1:
					sfx->Play(Player_ThisWillBeFun, Channel_Extra);
					break;
				}
			}
			backgroundMusic->Play(Music_Boss, Channel_Base);
			backgroundMusic->Play(Ambience_Lava, Channel_Extra);
			break;
		}
		case 8: //To stage 5
			backgroundMusic->Play(Music_Hot, Channel_Base);
			backgroundMusic->Play(Ambience_Lava, Channel_Extra);
			break;
		case 10: //To stage 6
			backgroundMusic->Play(Music_Hot, Channel_Base);
			backgroundMusic->Play(Ambience_Lava, Channel_Extra);
			break;
		case 12: //To stage 7
			backgroundMusic->Play(Music_Cold, Channel_Base);
			backgroundMusic->Play(Ambience_Blizzard, Channel_Extra);
			break;
		case 14: //To stage 8
			backgroundMusic->Play(Music_Cold, Channel_Base);
			backgroundMusic->Play(Ambience_Blizzard, Channel_Extra);
			break;
		case 16: //To stage 9
		{
			SoundComponent* sfx = registry.GetComponent<SoundComponent>(stateManager.player);
			if (sfx != nullptr)
			{
				int soundToPlay = rand() % 2;
				switch (soundToPlay) //Play player boss encounter sound
				{
				case 0:
					sfx->Play(Player_BringItOn, Channel_Extra);
					break;
				case 1:
					sfx->Play(Player_ThisWillBeFun, Channel_Extra);
					break;
				}
			}
			backgroundMusic->Play(Music_Boss, Channel_Base);
			backgroundMusic->Play(Ambience_Blizzard, Channel_Extra);
			break;
		}
		}
		audioJungle->HandleSound();
	}


	LoadLevel(++stateManager.activeLevel);
}

void UIFunctions::Game::ExitShopCutscene(void* args, int a)
{
	//Make the player fall
	if (Camera::InCutscene() == 3)//If in shop, do the falling
	{
		AudioEngineComponent* audioJungle;
		for (auto entity : View<AudioEngineComponent>(registry))
		{
			audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
		}

		PlayerComponent* playerComp = registry.GetComponent<PlayerComponent>(stateManager.player);
		UIPlayerSoulsComponent* playerSouls = registry.GetComponent<UIPlayerSoulsComponent>(stateManager.player);
		EntityID selectedID;
		if (playerSouls != nullptr)
		{
			if (playerSouls->spentThisShop == 0)
			{
				for (auto onClick : View<OnClickComponent>(registry))
				{
					OnClickComponent* shopBuy = registry.GetComponent<OnClickComponent>(onClick);
					if (shopBuy != nullptr)
					{
						for (int i = 0; i < (int)shopBuy->onClickFunctionsReleased.size(); i++)
						{
							if (shopBuy->onClickFunctionsReleased[i] == UIFunctions::OnClick::BuyRelic) //Purchase button found
							{
								selectedID = onClick;
							}
						}
					}
				}
			}
			playerSouls->spentThisShop = 0;
			playerSouls->spentThisShopOnRelics = 0;
		}
		//Play leave shop sounds
		if (selectedID.index != -1)
		{
			SoundComponent* sfx = registry.GetComponent<SoundComponent>(selectedID);
			if (sfx != nullptr) sfx->Play(Shop_BuyingNothing, Channel_Extra); //Play buy nothing sound.
		}

		CancelTimedEvents(stateManager.player);
		CutsceneComponent* fallDown = registry.AddComponent<CutsceneComponent>(stateManager.player);
		TransformComponent* transform = registry.GetComponent<TransformComponent>(stateManager.player);
		fallDown->mode = (CutsceneMode)(Cutscene_Character_Fall | Transition_Position | Cutscene_Accelerating);
		CutsceneSetPosition(stateManager.player, transform->positionX, transform->positionY, transform->positionZ, transform->positionX, -33.0f, transform->positionZ);
		AddTimedEventComponentStartContinuousEnd(stateManager.player, 0.0f, BeginPortalCutscene, CutsceneTransition, 5.0f, EventShopLoadNextLevel, CONDITION_IGNORE_GAMESPEED_SLOWDOWN, 2);
	}
	else //Otherwise (second time clicking) skip the shop cutscene
	{
		UIFunctions::Game::LoadNextLevel(nullptr, 0);
	}
}

void UIFunctions::Game::SetMainMenu(void* args, int a)
{
	SetInMainMenu(true);
	SetInPlay(false);
	UnloadEntities(ENT_PERSIST_LEVEL);
	gameSpeed = 1.0f;
	stateManager.menu.Setup();
}


void UIFunctions::Settings::Back(void* args, int a)
{
	//Please check this function cause the Setup sets in main menu to true already.
	//SetInMainMenu(true);
	stateManager.settings.Unload();
	stateManager.menu.Setup();
	SetInSettings(false);
}

void UIFunctions::Settings::SetLowRes(void* args, int a)
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

void UIFunctions::Settings::SetMediumRes(void* args, int a)
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

void UIFunctions::Settings::SetHighRes(void* args, int a)
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

void UIFunctions::Settings::SetFullscreen(void* args, int a)
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

void UIFunctions::Settings::SwitchTimer(void* args, int a)
{
	SetVisualTimer(!GetVisualTimer());

	for (auto entity : View<UIGameTimeComponent, UIComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UIGameTimeComponent* runTime = registry.GetComponent<UIGameTimeComponent>(entity);

		uiElement->m_BaseText.baseUI.SetVisibility(GetVisualTimer());
	}

	RedrawUI();
}

void UIFunctions::Settings::Volume::Press(void* args, int a)
{
	UISettingsSliderComponent* slider = registry.GetComponent<UISettingsSliderComponent>(*(EntityID*)args);

	slider->holding = true;
}

void UIFunctions::Settings::Volume::Release(void* args, int a)
{
	UISettingsSliderComponent* slider = registry.GetComponent<UISettingsSliderComponent>(*(EntityID*)args);

	slider->holding = false;
}

void UIFunctions::Credits_Back(void* args, int a)
{
	//Please check this function cause the Setup sets in main menu to true already.
	//SetInMainMenu(true);

	stateManager.credits.Unload();
	stateManager.menu.Setup();
	SetInCredits(false);
}


void UIFunctions::Pause::Resume(void* args, int a)
{
	if (currentStates & State::InShop)
	{
		SetInPause(false);
		SetInShop(true);

		SetPaused(false);

		RedrawUI();
		gameSpeed = 1.0f;
		ResetInput();

		UnloadEntities(ENT_PERSIST_PAUSE);
		if (Camera::InCutscene() > 0)
		{
			TimedEventIgnoreGamespeed(true);
			gameSpeed = 0.0f;
		}
	}
	else
	{
		SetInPause(false);
		SetInPlay(true);

		SetPaused(false);

		RedrawUI();
		gameSpeed = 1.0f;
		ResetInput();
		if (Camera::InCutscene() > 0)
		{
			TimedEventIgnoreGamespeed(true);
			gameSpeed = 0.0f;
		}
		UnloadEntities(ENT_PERSIST_PAUSE);
	}
}

void UIFunctions::Pause::SetSettings(void* args, int a)
{
	UIComponent* uiElement = registry.GetComponent<UIComponent>(*(EntityID*)args);
	UIFunctions::OnHover::Image(uiElement, 0, false);

	RedrawUI();
	for (auto entity : View<UIComponent, UIPauseButtonComponent>(registry))
	{
		UIComponent* uiPauseButton = registry.GetComponent<UIComponent>(entity);

		uiPauseButton->SetAllVisability(false);
	}

	for (auto entity : View<UIComponent, UIPauseSettingsComponent>(registry))
	{
		UIComponent* uiPauseSettings = registry.GetComponent<UIComponent>(entity);

		uiPauseSettings->SetAllVisability(true);
	}
}

void UIFunctions::Pause::Back(void* args, int a)
{
	UIComponent* uiElement = registry.GetComponent<UIComponent>(*(EntityID*)args);
	UIFunctions::OnHover::Image(uiElement, 0, false);

	RedrawUI();
	for (auto entity : View<UIComponent, UIPauseButtonComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

		uiElement->SetAllVisability(true);
	}

	for (auto entity : View<UIComponent, UIPauseSettingsComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

		uiElement->SetAllVisability(false);
	}
}

void UIFunctions::Pause::SetMainMenu(void* args, int a)
{
	//Please check this function cause the Setup sets in main menu to true already.
	//SetInMainMenu(true);
	UnloadEntities(ENT_PERSIST_LEVEL);
	gameSpeed = 1.0f;
	stateManager.menu.Setup();
	SetInPause(false);
}


void UIFunctions::OnClick::None(void* args, int index)
{

}

void UIFunctions::OnClick::SelectRelic(void* args, int index)
{
	UIComponent* uiElement = registry.GetComponent<UIComponent>(*(EntityID*)args);

	int inverseIndex = 0;
	int imageIndexes[2] = { 0, 0 };

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

	for (auto entity : View<UIShopRelicComponent>(registry))
	{
		UIComponent* otherUI = registry.GetComponent<UIComponent>(entity);

		if (uiElement != otherUI)
		{
			UIShopRelicComponent* otherWindow = registry.GetComponent<UIShopRelicComponent>(entity);
			for (uint32_t i = 0; i < otherWindow->shopSelections.size(); i++)
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

		UIShopRelicComponent* uiWindow = registry.GetComponent<UIShopRelicComponent>(entity);

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

void UIFunctions::OnClick::BuyRelic(void* args, int index)
{
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	UIPlayerSoulsComponent* souls = registry.GetComponent<UIPlayerSoulsComponent>(stateManager.player);
	UIPlayerRelicsComponent* playerRelics = registry.GetComponent<UIPlayerRelicsComponent>(stateManager.player);
	UIComponent* playerUI = registry.GetComponent<UIComponent>(stateManager.player);
	OnHoverComponent* playerHover = registry.GetComponent<OnHoverComponent>(stateManager.player);

	for (auto entity : View<UIShopRelicComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UIShopRelicComponent* relicWindow = registry.GetComponent<UIShopRelicComponent>(entity);

		for (int i = 0; i < 2; i++)
		{
			if (relicWindow->shopSelections[i] == shopState::SELECTED)
			{
				RelicInput::OnPriceCalculation priceCalc;

				for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
					func(&priceCalc);

				if (player->GetSouls() < priceCalc.GetCostOf(relicWindow->shopRelics[i]->m_price, RelicInput::OnPriceCalculation::RELIC))
					return;

				souls->spentThisShop += priceCalc.GetCostOf(relicWindow->shopRelics[i]->m_price, RelicInput::OnPriceCalculation::RELIC);
				souls->spentThisShopOnRelics += priceCalc.GetCostOf(relicWindow->shopRelics[i]->m_price, RelicInput::OnPriceCalculation::RELIC);
				player->UpdateSouls(-priceCalc.GetCostOf(relicWindow->shopRelics[i]->m_price, RelicInput::OnPriceCalculation::RELIC));

				relicWindow->shopSelections[i] = shopState::BOUGHT;

				uiElement->m_Images[i + 2].SetImage("Buy");
				relicWindow->shopRelics[i]->m_function(&stateManager.player);

				DSFLOAT2 offsetUICoords = { abs(playerUI->m_Images[2].baseUI.GetPixelCoords().x + 32.0f) ,
						   abs(playerUI->m_Images[2].baseUI.GetPixelCoords().y + 32.0f) };

				DSFLOAT2 uiPixelCoords = { (offsetUICoords.x / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
									-1 * ((offsetUICoords.y - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)) };

				//First relic purchase
				UIPlayerRelicsComponent* playerRelics = registry.GetComponent<UIPlayerRelicsComponent>(stateManager.player);
				if (playerRelics->currentRelics == 0)
				{
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
					if (sfx != nullptr) sfx->Play(Shop_FirstPurchase, Channel_Extra);
				}
				else if (stateManager.activeLevel == 6 || stateManager.activeLevel == 16)
				{
					if (souls->spentThisShopOnRelics == 0)
					{
						SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
						if (sfx != nullptr) sfx->Play(Shop_PurchaseBeforeBoss, Channel_Extra);
					}
				}

				if (playerRelics->currentRelics < playerRelics->maxRelics)
				{

					if (playerRelics->gridPos.x > 0)
					{
						playerRelics->gridPos.y++;
						playerRelics->gridPos.x = 0;
					}

					playerUI->AddImage(relicWindow->shopRelics[i]->m_filePath, DSFLOAT2(playerUI->m_Images[2].baseUI.GetPosition().x /*+(0.06f * playerRelics->gridPos.x)*/,
						uiPixelCoords.y - (0.12f * playerRelics->gridPos.y) - 0.02f), DSFLOAT2(1.5f, 1.5f), false);

					playerHover->Add(playerUI->m_Images[playerUI->m_Images.size() - 1].baseUI.GetPixelCoords(),
						playerUI->m_Images[playerUI->m_Images.size() - 1].baseUI.GetBounds(), UIFunctions::OnHover::PlayerRelic);

					playerRelics->relics[playerRelics->currentRelics] = relicWindow->shopRelics[i];

					playerRelics->gridPos.x++;
					playerRelics->currentRelics++;
				}

				int soundToPlay = rand() % 16;
				AudioEngineComponent* audioJungle = nullptr;
				for (auto jungle : View<AudioEngineComponent>(registry))
				{
					audioJungle = registry.GetComponent<AudioEngineComponent>(jungle);
				}
				bool isPlaying = false;

				if (soundToPlay == 0)
				{
					if (audioJungle != nullptr)
					{
						//Player voice line
						SoundComponent* sfx = registry.GetComponent<SoundComponent>(stateManager.player);
						if (sfx != nullptr)
						{
							audioJungle->channels[sfx->channelIndex[Channel_Extra]]->isPlaying(&isPlaying);
							if (!isPlaying)
							{
								soundToPlay = rand() % 2;
								if (soundToPlay == 0)
								{
									if (sfx != nullptr) sfx->Play(Player_BetterWork, Channel_Extra);
								}
								else
								{
									if (sfx != nullptr) sfx->Play(Player_SomethingPositive, Channel_Extra);
								}
							}
						}
					}
				}
				else if (soundToPlay == 1)
				{
					if (audioJungle != nullptr)
					{
						//Imp voice line
						SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
						if (sfx != nullptr)
						{
							audioJungle->channels[sfx->channelIndex[Channel_Extra]]->isPlaying(&isPlaying);
							if (!isPlaying)
							{
								soundToPlay = rand() % 2;
								if (soundToPlay == 0)
								{
									if (sfx != nullptr) sfx->Play(Shop_RelicPurchase, Channel_Extra);
								}
								else
								{
									if (sfx != nullptr) sfx->Play(Shop_RelicPurchase2, Channel_Extra);
								}
							}
						}
					}
				}

				//Normal buy sound
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
				if (sfx != nullptr) sfx->Play(Shop_Buy, Channel_Base);

				break;
			}
		}
	}

	RedrawUI();
}

void UIFunctions::OnClick::LockRelic(void* args, int index)
{
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	UIPlayerSoulsComponent* souls = registry.GetComponent<UIPlayerSoulsComponent>(stateManager.player);

	if (player != nullptr && player->GetSouls() < 0)
		return;
	
	for (auto entity : View<UIShopRelicComponent>(registry))
	{
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);
		UIShopRelicComponent* relicWindow = registry.GetComponent<UIShopRelicComponent>(entity);

		for (int i = 0; i < 2; i++)
		{
			if (relicWindow->shopSelections[i] == shopState::SELECTED)
			{
				relicWindow->shopSelections[i] = shopState::LOCKED;
				uiElement->m_Images[i + 2].SetImage("RelicIcons\\LockedRelic");
				player->UpdateSouls(0);

				//Normal lock sound
				SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
				if (sfx != nullptr) sfx->Play(Shop_Dibs, Channel_Base);

				souls->spentThisShop += 0;
				break;
			}
		}
	}
}

void UIFunctions::OnClick::UpgradeWeapon(void* args, int index)
{
	UIShopButtonComponent* uiWeapon = registry.GetComponent<UIShopButtonComponent>(*(EntityID*)args);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	StatComponent* stats = registry.GetComponent<StatComponent>(stateManager.player);

	RelicInput::OnPriceCalculation priceCalc;

	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
		func(&priceCalc);

	if (player->GetSouls() < priceCalc.GetCostOf(uiWeapon->m_price, RelicInput::OnPriceCalculation::UPGRADE))
		return;

	player->UpdateSouls(-priceCalc.GetCostOf(uiWeapon->m_price, RelicInput::OnPriceCalculation::UPGRADE));
	stats->UpdateBaseDamage(uiWeapon->m_price - 2);
}

void UIFunctions::OnClick::RerollRelic(void* args, int index)
{
	UIShopButtonComponent* uiReroll = registry.GetComponent<UIShopButtonComponent>(*(EntityID*)args);

	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	UIPlayerSoulsComponent* souls = registry.GetComponent<UIPlayerSoulsComponent>(stateManager.player);

	RelicInput::OnPriceCalculation priceCalc;

	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
		func(&priceCalc);

	if (player != nullptr && player->GetSouls() < priceCalc.GetCostOf(uiReroll->m_price, RelicInput::OnPriceCalculation::REROLL))
		return;


	for (auto entity : View<UIShopRelicComponent>(registry))
	{
		UIComponent* uiRelic = registry.GetComponent<UIComponent>(entity);
		UIShopRelicComponent* relicWindow = registry.GetComponent<UIShopRelicComponent>(entity);

		for (int i = 0; i < 2; i++)
		{
			bool ignore = false;
			if (relicWindow->shopSelections[i] == shopState::BOUGHT)
				ignore = true;

			uiRelic->m_Images[i + 2].SetImage("RelicIcons\\HoverRelic");
			uiRelic->m_Images[i + 2].baseUI.SetVisibility(false);

			if (relicWindow->shopSelections[i] == shopState::LOCKED)
			{
				relicWindow->shopSelections[i] = shopState::AVALIABLE;
				continue;
			}

			if (!ignore)
				Relics::PutBackRelic(relicWindow->shopRelics[i]);

			relicWindow->shopSelections[i] = shopState::AVALIABLE;

			ML_Map<ML_String, Relics::RELIC_TYPE> type;
			type.emplace("Offence", Relics::RELIC_OFFENSE);
			type.emplace("Defence", Relics::RELIC_DEFENSE);
			type.emplace("Gadget", Relics::RELIC_GADGET);

			const RelicData* relic = Relics::PickRandomRelic(type[uiRelic->m_BaseText.m_Text]);
			uiRelic->m_Images[i].SetImage(relic->m_filePath);
			relicWindow->shopRelics[i] = relic;
		}
	}

	//Normal re-reroll sound
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
	if (sfx != nullptr) sfx->Play(Shop_Reroll, Channel_Base);

	souls->spentThisShop += priceCalc.GetCostOf(uiReroll->m_price, RelicInput::OnPriceCalculation::REROLL);
	player->UpdateSouls(-priceCalc.GetCostOf(uiReroll->m_price, RelicInput::OnPriceCalculation::REROLL));

	RedrawUI();
}

void UIFunctions::OnClick::HealPlayer(void* args, int index)
{
	UIShopButtonComponent* uiHeal = registry.GetComponent<UIShopButtonComponent>(*(EntityID*)args);
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(stateManager.player);
	UIPlayerSoulsComponent* souls = registry.GetComponent<UIPlayerSoulsComponent>(stateManager.player);
	StatComponent* stats = registry.GetComponent<StatComponent>(stateManager.player);

	RelicInput::OnPriceCalculation priceCalc;

	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
		func(&priceCalc);

	if (player->GetSouls() < priceCalc.GetCostOf(uiHeal->m_price, RelicInput::OnPriceCalculation::HEAL))
		return;

	if (stats->GetHealth() == stats->GetMaxHealth())
		return;

	float heal = stats->GetMaxHealth() * 0.25f;

	//Normal heal sound
	SoundComponent* sfx = registry.GetComponent<SoundComponent>(*(EntityID*)args);
	if (sfx != nullptr) sfx->Play(Shop_Heal, Channel_Base);

	stats->ApplyHealing(heal);

	souls->spentThisShop += priceCalc.GetCostOf(uiHeal->m_price, RelicInput::OnPriceCalculation::HEAL);
	player->UpdateSouls(-priceCalc.GetCostOf(uiHeal->m_price, RelicInput::OnPriceCalculation::HEAL));
}


void UIFunctions::OnHover::None(void* args, int index, bool hover)
{

}

void UIFunctions::OnHover::Image(void* args, int index, bool hover)
{
	UIComponent* uiElement = (UIComponent*)args;
	ML_String fileName = "";
	ML_String hoverFileName = "";
	if (uiElement->m_Images.size() == 0)
	{
		if (uiElement->m_BaseImage.baseUI.GetVisibility())
		{
			fileName = uiElement->m_BaseImage.m_fileName;
			hoverFileName = fileName;
			hoverFileName.append("Hover");

			if (hover)
				uiElement->m_BaseImage.SetImage(hoverFileName.c_str(), true);
			else
				uiElement->m_BaseImage.SetImage(fileName.c_str(), true);
		}
	}
	else if (uiElement->m_Images[index].baseUI.GetVisibility())
	{
		fileName = uiElement->m_Images[index].m_fileName;
		hoverFileName = fileName;
		hoverFileName.append("Hover");

		if (hover)
			uiElement->m_Images[index].SetImage(hoverFileName.c_str(), true);
		else
			uiElement->m_Images[index].SetImage(fileName.c_str(), true);
		
	}
}

void UIFunctions::OnHover::ShopButton(void* args, int index, bool hover)
{
	UIComponent* uiElement = (UIComponent*)args;
	UIShopButtonComponent* shopButton = nullptr;

	if (!uiElement->m_BaseImage.baseUI.GetVisibility())
		return;

	UIComponent* uiImpElement = nullptr;
	UIShopImpComponent* uiImpText = nullptr;

	for (auto entity : View<UIShopButtonComponent>(registry))
	{
		UIComponent* otherUI = registry.GetComponent<UIComponent>(entity);

		if (uiElement != otherUI)
			continue;

		shopButton = registry.GetComponent<UIShopButtonComponent>(entity);

	}

	for (auto entity : View<UIShopImpComponent>(registry))
	{
		uiImpElement = registry.GetComponent<UIComponent>(entity);
		uiImpText = registry.GetComponent<UIShopImpComponent>(entity);
	}

	if (uiImpElement == nullptr || uiImpText == nullptr)
		return;

	if (hover)
	{
		uiImpText->name = shopButton->m_name;
		uiImpText->description = shopButton->m_description;
		
		RelicInput::OnPriceCalculation priceCalc;

		for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
			func(&priceCalc);

		if (shopButton->m_name == "Heal")
		{
			uiImpText->price = priceCalc.GetCostOf(shopButton->m_price, RelicInput::OnPriceCalculation::HEAL);
		}
		else if (shopButton->m_name == "Reroll")
		{
			uiImpText->price = priceCalc.GetCostOf(shopButton->m_price, RelicInput::OnPriceCalculation::REROLL);
		}
		else if (shopButton->m_name == "Upgrade Weapon")
		{
			uiImpText->price = priceCalc.GetCostOf(shopButton->m_price, RelicInput::OnPriceCalculation::UPGRADE);
		}
		else
			uiImpText->price = shopButton->m_price;

		ML_String buttonText = uiImpText->name;

		if (uiImpText->price > 0)
		{
			buttonText.append("\nPrice: ");
			buttonText.append((std::to_string(uiImpText->price) + " Souls\n").c_str());
		}
		else
		{
			if (shopButton->m_name == "Lock" || shopButton->m_name == "Buy")
			{
				buttonText.append("\n");
			}
			else
				buttonText.append("\nPrice: Free\n");

		}

		buttonText.append(uiImpText->description);

		uiImpElement->m_BaseText.SetText(buttonText.c_str(), uiImpElement->m_BaseText.baseUI.GetBounds(), uiImpElement->m_BaseText.m_fontSize,
			uiImpElement->m_BaseText.m_textAlignment, uiImpElement->m_BaseText.m_paragraphAlignment);


		UIFunctions::OnHover::Image(args, index, hover);

	}
	else
	{
		UIFunctions::OnHover::Image(args, index, hover);
	}

}

void UIFunctions::OnHover::ShopRelic(void* args, int index, bool hover)
{
	UIComponent* uiElement = (UIComponent*)args;
	UIShopRelicComponent* relicWindow = nullptr;

	if (!uiElement->m_BaseImage.baseUI.GetVisibility())
		return;

	UIComponent* uiImpElement = nullptr;
	UIShopImpComponent* uiImpText = nullptr;

	for (auto entity : View<UIShopRelicComponent>(registry))
	{
		UIComponent* otherUI = registry.GetComponent<UIComponent>(entity);

		if (uiElement != otherUI)
			continue;

		relicWindow = registry.GetComponent<UIShopRelicComponent>(entity);
		
	}

	for (auto entity : View<UIShopImpComponent>(registry))
	{
		uiImpElement = registry.GetComponent<UIComponent>(entity);
		uiImpText = registry.GetComponent<UIShopImpComponent>(entity);
	}

	if (uiImpElement == nullptr || uiImpText == nullptr)
		return;

	int imageIndex = uiElement->m_Images.size() - (2 - index + 1);

	if (hover)
	{
		uiElement->m_Images[imageIndex].baseUI.SetVisibility(true);
		uiElement->m_Images[imageIndex].baseUI.SetPosition(uiElement->m_Images[index - 1].baseUI.GetPosition());

		uiImpText->name = relicWindow->shopRelics[index - 1]->m_relicName;
		uiImpText->description = relicWindow->shopRelics[index - 1]->m_description;

		RelicInput::OnPriceCalculation priceCalc;

		for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_PRICE_CALC))
			func(&priceCalc);

		uiImpText->price = priceCalc.GetCostOf(relicWindow->shopRelics[index - 1]->m_price, RelicInput::OnPriceCalculation::RELIC);

		ML_String relicText = uiImpText->name;

		if (uiImpText->price > 0)
		{
			
			relicText.append("\nPrice: ");
			relicText.append((std::to_string(uiImpText->price) + " Souls\n").c_str());
		}
		else
			relicText.append("\nPrice: Free\n");

		relicText.append(uiImpText->description);


		uiImpElement->m_BaseText.SetText(relicText.c_str(), uiImpElement->m_BaseText.baseUI.GetBounds(), uiImpElement->m_BaseText.m_fontSize,
			uiImpElement->m_BaseText.m_textAlignment, uiImpElement->m_BaseText.m_paragraphAlignment);
	}
	else if (!hover && relicWindow->shopSelections[index - 1] == shopState::AVALIABLE)
	{
		uiElement->m_Images[imageIndex].baseUI.SetVisibility(false);
	}

}

void UIFunctions::OnHover::PlayerRelic(void* args, int index, bool hover)
{
	UIComponent* uiElement = (UIComponent*)args;
	UIPlayerRelicsComponent* relicWindow = registry.GetComponent<UIPlayerRelicsComponent>(stateManager.player);

	if (!uiElement->m_BaseImage.baseUI.GetVisibility())
		return;

	ML_String relicText = relicWindow->relics[index]->m_relicName;
	relicText.append("\n");
	relicText.append(relicWindow->relics[index]->m_description);

	if ((currentStates & State::InShop))
	{
		UIComponent* uiImpElement = nullptr;
		UIShopImpComponent* uiImpText = nullptr;

		for (auto entity : View<UIShopImpComponent>(registry))
		{
			uiImpElement = registry.GetComponent<UIComponent>(entity);
			uiImpText = registry.GetComponent<UIShopImpComponent>(entity);
		}

		if (uiImpElement == nullptr || uiImpText == nullptr)
			return;

		uiImpText->name = relicWindow->relics[index]->m_relicName;
		uiImpText->description = relicWindow->relics[index]->m_description;
		uiImpText->price = relicWindow->relics[index]->m_price;

		uiImpElement->m_BaseText.SetText(relicText.c_str(), uiImpElement->m_BaseText.baseUI.GetBounds());
	}
	else if ((currentStates & State::InPause))
	{
		UIComponent* uiPauseElement = nullptr;
		UIPauseRelicTextComponent* uiPauseText = nullptr;

		for (auto entity : View<UIPauseRelicTextComponent>(registry))
		{
			uiPauseElement = registry.GetComponent<UIComponent>(entity);
			uiPauseText = registry.GetComponent<UIPauseRelicTextComponent>(entity);
		}

		if (uiPauseElement == nullptr || uiPauseText == nullptr)
			return;

		uiPauseText->name = relicWindow->relics[index]->m_relicName;
		uiPauseText->description = relicWindow->relics[index]->m_description;

		uiPauseElement->m_Images[0].baseUI.SetPosition({ uiElement->m_Images[2].baseUI.GetPosition().x + 0.3f, uiElement->m_Images[2].baseUI.GetPosition().y });
		uiPauseElement->m_Texts[0].baseUI.SetPosition(uiPauseElement->m_Images[0].baseUI.GetPosition());

		uiPauseElement->m_Texts[0].SetText(relicText.c_str(), uiPauseElement->m_BaseText.baseUI.GetBounds());

		if (hover)
		{
			uiPauseElement->m_Images[0].baseUI.SetVisibility(true);
			uiPauseElement->m_Texts[0].baseUI.SetVisibility(true);
		}
		else
		{
			uiPauseElement->m_Images[0].baseUI.SetVisibility(false);
			uiPauseElement->m_Texts[0].baseUI.SetVisibility(false);
		}

	}
}

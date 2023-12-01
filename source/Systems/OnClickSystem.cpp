#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "UI/UIRenderer.h"
#include "SDLHandler.h"
#include "UIButtonFunctions.h"
#include "UIComponents.h"

#include "OnClickComponent.h"

bool OnClickSystem::Update()
{
	for (auto entity : View<OnClickComponent, UIComponent>(registry))
	{
		auto comp = registry.GetComponent<OnClickComponent>(entity);
		auto uiElement = registry.GetComponent<UIComponent>(entity);

		int index = comp->Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) });

		if (comp->onClickFunctionsPressed.size() == 0 || comp->onClickFunctionsReleased.size() == 0)
			continue;

		if (mouseButtonPressed[MouseButton::left] == pressed && index > -1)
		{
			if (index == 0) //baseimage intersect
			{
				if (!uiElement->m_BaseImage.baseUI.GetVisibility())
					continue;
			}
			else if (index > 0) //images intersect, higher number = later added image
			{
				if (!uiElement->m_Images[index - 1].baseUI.GetVisibility())
					continue;
			}

			comp->onClickFunctionsPressed[comp->index](&entity, comp->index);
			return true;
		}
		else if (mouseButtonPressed[MouseButton::right] == pressed && index > -1)
		{

		}
		else if (mouseButtonPressed[MouseButton::middle] == pressed && index > -1)
		{

		}

		if (mouseButtonPressed[MouseButton::left] == released && index > -1)
		{
			if (index == 0) //baseimage intersect
			{
				if (!uiElement->m_BaseImage.baseUI.GetVisibility())
					continue;
			}
			else if (index > 0) //images intersect, higher number = later added image
			{
				if (!uiElement->m_Images[index - 1].baseUI.GetVisibility())
					continue;
			}

			//Set which sound to play
			SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
			if (sound != nullptr)
			{
				if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::MainMenu::Start)
				{
					//Stop all the previous sounds (except music) to mute for example the dog breath
					for (auto entity : View<AudioEngineComponent>(registry))
					{
						AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
						audioJungle->HandleSound();
						audioJungle->StopAllSounds();
					}
					sound->Play(Button_Start, Channel_Base);
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::OnClick::SelectRelic)
				{
					sound->Play(Shop_Press, Channel_Base);
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::OnClick::BuyRelic)
				{
					//Play sound in the function itself
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::OnClick::HealPlayer)
				{
					//Play sound in the function itself
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::OnClick::LockRelic)
				{
					//Play sound in the function itself
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::OnClick::RerollRelic)
				{
					//Play sound in the function itself
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::OnClick::UpgradeWeapon)
				{
					//Play sound in the function itself
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::Game::LoadNextLevel)
				{
					for (auto entity : View<AudioEngineComponent>(registry))
					{
						AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
						audioJungle->HandleSound();
						audioJungle->StopAllSounds();
					}
				}
				else if (comp->onClickFunctionsReleased[comp->index] == UIFunctions::Game::SetMainMenu)
				{
					//Stop all the previous sounds (except music) to mute for example the dog breath
					for (auto entity : View<AudioEngineComponent>(registry))
					{
						AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
						audioJungle->HandleSound();
						audioJungle->StopAllSounds();
					}
					sound->Play(Button_Press, Channel_Base);
				}
				//else if (comp->onClickFunctionsReleased[comp->index] == UIFunc::UpgradeWeapon) //Add this when the function for upgrading weapon has been included.
				else if (comp->onClickFunctionsReleased[comp->index] != UIFunctions::OnClick::None)
				{
					sound->Play(Button_Press, Channel_Base);
				}
			}

			comp->onClickFunctionsReleased[comp->index](&entity, comp->index);
			return true;
		}
		else if (mouseButtonPressed[MouseButton::right] == released && index > -1)
		{

		}
		else if (mouseButtonPressed[MouseButton::middle] == released && index > -1)
		{

		}

		
	}

	return true;
}
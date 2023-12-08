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

#include "OnHoverComponent.h"

void UndrawHoverInteractable(OnHoverComponent& comp, UIComponent& uiElement, int index)
{
	if (comp.hasBeenDrawnChecks[index] == 1)
	{
		RedrawUI();
		comp.onHoverFunctions[index](&uiElement, index, false);
		comp.hasBeenDrawnChecks[index] = 0;
		comp.redrawUIChecks[index] = 1;
	}
};


bool OnHoverSystem::Update()
{
	for (auto entity : View<OnHoverComponent, UIComponent>(registry))
	{
		OnHoverComponent* comp = registry.GetComponent<OnHoverComponent>(entity);
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

		if (comp->hasBeenDrawnChecks.size() == 0)
			continue;

		int index = comp->Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) });

		//compare old interaction vs new to see if mouse has been moved off of the interactable
		if (index != comp->oldIndex)
		{
			if (index == -1) //mouse moved to no image
			{
				if (comp->hasBeenDrawnChecks[comp->index] == 1)
					UndrawHoverInteractable(*comp, *uiElement, comp->index);
			}
			else if (index > -1) //mouse moved to another image
			{
				if (comp->hasBeenDrawnChecks[comp->oldIndex] == 1)
					UndrawHoverInteractable(*comp, *uiElement, comp->oldIndex);
			}
		}

		if (index != -1)
			comp->oldIndex = index;


		if (index == comp->index)
		{
			int imageIndex = (index - 1) * 3;
			//skip if interactable isnt visible or has no hover function
			if ((index == 0 && !uiElement->m_BaseImage.baseUI.GetVisibility()) || (index == 0 && comp->onHoverFunctions[index] == UIFunctions::OnHover::None))
				continue;
			else if ((index > 0 && !uiElement->m_Images[imageIndex].baseUI.GetVisibility()) || (index > 0 && comp->onHoverFunctions[index] == UIFunctions::OnHover::None))
				continue;


			if (comp->hasBeenDrawnChecks[comp->index] == 0 && comp->redrawUIChecks[comp->index] == 1)
			{
				//Set which sound to play
				SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				OnClickComponent* onClick = registry.GetComponent<OnClickComponent>(entity);

				//Check if the channel is currently playing a sound or not
				for (auto entity : View<AudioEngineComponent>(registry))
				{
					AudioEngineComponent* audioJungle = registry.GetComponent<AudioEngineComponent>(entity);
					bool isPlaying = false;
					if (sound != nullptr)
					{
						audioJungle->channels[sound->channelIndex[0]]->isPlaying(&isPlaying);
						FMOD::Sound* test = nullptr;
						audioJungle->channels[sound->channelIndex[0]]->getCurrentSound(&test);
						if (audioJungle->sounds[MENU1] != test)
						{
							if ((onClick != nullptr) && (!isPlaying)) sound->Play(Button_Hover, Channel_Base);
						}
						else
						{
							if (onClick != nullptr) sound->Play(Button_Hover, Channel_Base);
						}
					}
				}

				RedrawUI();
				comp->redrawUIChecks[comp->index] = 0;
				comp->hasBeenDrawnChecks[comp->index] = 1;
				comp->onHoverFunctions[comp->index](uiElement, index, true);
			}
		}

	}


	return true;
}
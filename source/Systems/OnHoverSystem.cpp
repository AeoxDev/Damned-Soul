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
	if (comp.hasBeenDrawnChecks[index] == true)
	{
		RedrawUI();
		comp.onHoverFunctions[index](&uiElement, index, false);
		comp.hasBeenDrawnChecks[index] = false;
		comp.redrawUIChecks[index] = true;
	}
};

bool OnHoverSystem::Update()
{
	for (auto entity : View<OnHoverComponent, UIComponent>(registry))
	{
		OnHoverComponent* comp = registry.GetComponent<OnHoverComponent>(entity);
		UIComponent* uiElement = registry.GetComponent<UIComponent>(entity);

		int index = comp->Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) });

		//compare old interaction vs new to see if mouse has been moved off of the interactable
		if (index != comp->oldIndex)
		{
			if (index == -1) //mouse moved to no image
			{
				if (comp->hasBeenDrawnChecks[comp->index] == true)
				{
					UndrawHoverInteractable(*comp, *uiElement, comp->index);
				}
			}
			else if (index > -1) //mouse moved to another image
			{
				if (comp->hasBeenDrawnChecks[comp->oldIndex] == true)
				{
					UndrawHoverInteractable(*comp, *uiElement, comp->oldIndex);
				}
			}
		}


		if (index != -1)
			comp->oldIndex = index;


		if (index == comp->index)
		{
			//skip if interactable isnt visible or has no hover function
			if ((index == 0 && !uiElement->m_BaseImage.baseUI.GetVisibility()) || (index == 0 && comp->onHoverFunctions[index] == UIFunc::EmptyOnHover))
			{
				continue;
			}
			else if ((index > 0 && !uiElement->m_Images[index - 1].baseUI.GetVisibility()) || (index > 0 && comp->onHoverFunctions[index] == UIFunc::EmptyOnHover))
			{
				continue;
			}


			if (comp->hasBeenDrawnChecks[comp->index] == false && comp->redrawUIChecks[comp->index] == true)
			{
				//Set which sound to play
				SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				if (sound != nullptr) sound->Play(Button_Hover, Channel_Base);

				RedrawUI();
				comp->redrawUIChecks[comp->index] = false;
				comp->hasBeenDrawnChecks[comp->index] = true;
				comp->onHoverFunctions[comp->index](uiElement, index, true);
			}
		}

	}


	return true;
}
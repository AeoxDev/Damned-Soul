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

bool OnHoverSystem::Update()
{
	for (auto entity : View<OnHoverComponent, UIComponent>(registry))
	{
		auto comp = registry.GetComponent<OnHoverComponent>(entity);
		auto ui = registry.GetComponent<UIComponent>(entity);
		if (comp->Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
		{
			if (!comp->hasBeenDrawn && comp->redrawUI)
			{
				//Set which sound to play
				SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				if (sound != nullptr) sound->Play(Button_Hover, Channel_Base);

				RedrawUI();
				comp->redrawUI = false;
				comp->hasBeenDrawn = true;
				comp->onHover(ui, true);
			}
		}
		else
		{
			if (comp->hasBeenDrawn)
			{
				RedrawUI();
				comp->onHover(ui, false);
				comp->hasBeenDrawn = false;
				comp->redrawUI = true;
			}

		}
	}

	return true;
}
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
	for (auto entity : View<OnClickComponent>(registry))
	{
		auto comp = registry.GetComponent<OnClickComponent>(entity);
		if (comp->Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
		{
			/*SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				if (sound != nullptr) sound->Play(Button_Hover, Channel_Base);*/

			if (mouseButtonPressed[MouseButton::left] == released)
			{
				//Set which sound to play
				SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				if (sound != nullptr)
				{
					if (comp->onClick == UIFunc::LoadNextLevel)	
					{
						sound->Play(Button_Start, Channel_Base);
					}
					else
					{
						sound->Play(Button_Press, Channel_Base);
					}
				}

				comp->onClick(comp);
				return true;
			}
		}
	}

	return true;
}
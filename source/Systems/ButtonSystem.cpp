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

bool ButtonSystem::Update()
{
	for (auto entity : View<UIButton>(registry))
	{
		auto comp = registry.GetComponent<UIButton>(entity);
		if (comp->m_UiComponent.Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
		{
			if (comp->m_CurrentImage == 1 && comp->doRedraw)
			{
				//Set which sound to play
				SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				if (sound != nullptr) sound->Play(Button_Hover, Channel_Base);

				RedrawUI();
				comp->doRedraw = false;
			}

			comp->Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
			{
				//Set which sound to play
				SoundComponent* sound = registry.GetComponent<SoundComponent>(entity);
				if (sound != nullptr)
				{
					if (comp->m_onClick == UIFunc::MainMenu_Start)
					{
						sound->Play(Button_Start, Channel_Base);
					}
					else
					{
						sound->Play(Button_Press, Channel_Base);
					}
				}

				comp->Interact();
				return true;
			}
		}
		else
		{
			if (comp->m_CurrentImage == 1)
			{
				RedrawUI();
				comp->m_CurrentImage = 0;
				comp->doRedraw = true;
			}
			
		}
	}
	return true;
}
#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "UI/UIRenderer.h"
#include "SDLHandler.h"
#include "UIComponents.h"

bool ButtonSystem::Update()
{
	for (auto entity : View<UIButton>(registry))
	{
		auto comp = registry.GetComponent<UIButton>(entity);
		if (comp->m_UiComponent.Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
		{
			if (comp->m_CurrentImage == 1 && comp->m_doRedraw)
			{
				RedrawUI();
				comp->m_doRedraw = false;
			}

			comp->Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
			{
				comp->Interact(comp);
				return true;
			}
		}
		else
		{
			if (comp->m_CurrentImage == 1)
			{
				RedrawUI();
				comp->m_CurrentImage = 0;
				comp->m_doRedraw = true;
			}
			
		}
	}

	return true;
}
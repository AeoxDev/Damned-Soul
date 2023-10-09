#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "UI/UIRenderer.h"
#include "SDLHandler.h"

bool ButtonSystem::Update()
{
	for (auto entity : View<UIButtonComponent>(registry))
	{
		auto comp = registry.GetComponent<UIButtonComponent>(entity);
		if (comp->button.m_uiComponent.Intersect({ (int)((float)mouseX * ((float)sdl.BASE_WIDTH / (float)sdl.WIDTH)), (int)((float)mouseY * ((float)sdl.BASE_HEIGHT / (float)sdl.HEIGHT)) }))
		{
			RedrawUI();
			comp->button.Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
				comp->button.Interact();
		}
	}
	return true;
}
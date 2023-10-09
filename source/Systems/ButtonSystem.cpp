#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "UI/UIRenderer.h"

bool ButtonSystem::Update()
{
	for (auto entity : View<UIButtonComponent>(registry))
	{
		auto comp = registry.GetComponent<UIButtonComponent>(entity);
		if (comp->button.m_uiComponent.Intersect({ mouseX, mouseY }))
		{
			if (comp->button.m_CurrentImage == 1 && comp->doRedraw)
			{
				RedrawUI();
				comp->doRedraw = false;
			}

			comp->button.Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
			{
				comp->button.Interact();
				return true;
			}
		}
		else
		{
			if (comp->button.m_CurrentImage == 1)
			{
				RedrawUI();
				comp->button.m_CurrentImage = 0;
				comp->doRedraw = true;
			}
			
		}
	}
	return true;
}
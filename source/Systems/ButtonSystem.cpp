#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"
#include "UI/UIRenderer.h"

bool ButtonSystem::Update()
{
	for (auto entity : View<UIButton>(registry))
	{
		auto comp = registry.GetComponent<UIButton>(entity);
		if (comp->m_UiComponent.Intersect({ mouseX, mouseY }))
		{
			if (comp->m_CurrentImage == 1 && comp->doRedraw)
			{
				RedrawUI();
				comp->doRedraw = false;
			}

			comp->Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
			{
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
#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"

bool ButtonSystem::Update()
{
	for (auto entity : View<ButtonComponent>(registry))
	{
		auto comp = registry.GetComponent<ButtonComponent>(entity);
		if (comp->button.m_uiComponent.Intersect({ mouseX, mouseY }))
		{
			comp->button.Hover();
			if (mouseButtonPressed[MouseButton::left] == released)
				comp->button.Interact();
		}
	}
	return true;
}
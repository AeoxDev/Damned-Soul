#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "Input.h"

bool ButtonSystem::Update()
{
	for (auto entity : View<UICanvas>(registry))
	{
		UICanvas* canvas = registry.GetComponent<UICanvas>(ui);
		//Input controller component.
		if (mouseButtonPressed[MouseButton::left] == released && canvas->m_Buttons[0].m_uiComponent.Intersect({ mouseX, mouseY }))
		{

			//Unload();
		}
		if (mouseButtonPressed[MouseButton::left] == released && canvas->m_Buttons[1].m_uiComponent.Intersect({ mouseX, mouseY }))
		{
			//Unload();
		}
		
	}
	return true;
}
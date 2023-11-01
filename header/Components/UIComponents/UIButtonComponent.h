#pragma once
#include "UIElement.h"

struct UIButtonComponent
{
	UIComponent uiElement;
	void(*onClick)(void*);
	void(*onHover)(void*);

};
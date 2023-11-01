#pragma once
#include "UIElement.h"

struct UIRelicComponent
{
	UIComponent uiElement;

	int locked = -1;
	int relicIndex = -1;
	int price = -1;
};

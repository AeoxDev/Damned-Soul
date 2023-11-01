#pragma once
#include "UIElement.h"
#include "UIButtonFunctions.h"
#include "RelicFunctions.h"

#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

#include "Registry.h"

struct EntityID;

struct UIShopRelicWindowComponent
{
	UIComponent uiElement;

	bool doRedraw = true;
	int shopPosition = -1;
	bool bought = false;

	void Setup(int position, UIImage baseImage, UIText priceText)
	{
		shopPosition = position;
		uiElement.m_BaseImage = baseImage;
		uiElement.m_Text = priceText;
	};
};

struct UIShopComponent
{
	UIComponent uiElement;

	bool ignore = true;

	UIShopComponent();

	void Setup();
};


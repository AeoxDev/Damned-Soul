#pragma once


#include "UI\UIImage.h"
#include "UI\UIText.h"
#include "UI\UIButton.h"
#include "UIButtonFunctions.h"
#include "RelicFunctions.h"

#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

#include "Registry.h"

struct EntityID;

struct UIShopRelicWindowComponent
{
	DSFLOAT2 position = { 0.0f, 0.0f };
	DSFLOAT2 scale = { 1.0f, 1.0f };

	bool doRedraw = true;

	int shopPosition = -1;

	bool bought = false;

	UIImage m_baseImage;
	UIText m_priceText;

	void Setup(int position, UIImage baseImage, UIText priceText)
	{
		shopPosition = position;
		m_baseImage = baseImage;
		m_priceText = priceText;
	};
};

struct UIShopComponent
{
	DSFLOAT2 position = { 0.0f, 0.0f };
	DSFLOAT2 scale = { 1.0f, 1.0f };

	UIImage baseImage;
	UIText playerInfo;

	bool ignore = true;

	ML_Vector<Relics::RelicMetaData> relics;

	UIShopComponent();

	void Setup();
};


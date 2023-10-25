#pragma once
#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIRelicComponent
{
	DSFLOAT2 position;
	DSFLOAT2 scale;

	bool doRedraw = true;
	int locked = -1;
	int relicIndex = -1;
	int price = -1;

	UIImage sprite;
	UIImage flavorTitleImage;
	UIText flavorTitle;
	UIImage flavorDescImage;
	UIText flavorDesc;

	UIRelicComponent(UIImage sprite, UIImage flavorTitleImage, UIText flavorTitle, UIImage flavorDescImage, UIText flavorDesc, DSFLOAT2 position = { 0.0f, 0.0f }, DSFLOAT2 scale = { 1.0f, 1.0f })
		:position(position), scale(scale), sprite(sprite), flavorTitleImage(flavorTitleImage), flavorTitle(flavorTitle), flavorDescImage(flavorDescImage), flavorDesc(flavorDesc) {}
};

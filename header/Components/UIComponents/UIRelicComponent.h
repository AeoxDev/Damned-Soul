#pragma once
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"

#include "RelicFunctions.h"

struct UIRelicComponent
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;

	bool doRedraw = true;
	int locked = -1;
	int relicIndex = -1;

	UIImage sprite;
	UIImage flavorTitleImage;
	UIText flavorTitle;
	UIImage flavorDescImage;
	UIText flavorDesc;

	UIRelicComponent(UIImage sprite, UIImage flavorTitleImage, UIText flavorTitle, UIImage flavorDescImage, UIText flavorDesc, DirectX::XMFLOAT2 position = { 0.0f, 0.0f }, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f })
		:position(position), scale(scale), sprite(sprite), flavorTitleImage(flavorTitleImage), flavorTitle(flavorTitle), flavorDescImage(flavorDescImage), flavorDesc(flavorDesc) {}
};

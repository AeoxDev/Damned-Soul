#pragma once
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIRelicComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 0.0f, 0.0f };

	bool doRedraw = true;

	UIImage sprite;
	UIImage flavorImage;
	UIText flavorTitle;
	UIText flavorText;

	UIRelicComponent(UIImage sprite, UIImage flavorImage, UIText flavorTitle, UIText flavorText)
		:position(position), scale(scale), sprite(sprite), flavorImage(flavorImage), flavorTitle(flavorTitle), flavorText(flavorText) {}
};

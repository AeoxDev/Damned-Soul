#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIRelicComponent
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;

	bool doRedraw = true;

	UIImage sprite;
	UIImage flavorImage;
	UIText flavorTitle;
	UIText flavorText;

	UIRelicComponent(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, UIImage sprite, UIImage flavorImage, UIText flavorTitle, UIText flavorText)
		:position(position), scale(scale), sprite(sprite), flavorImage(flavorImage), flavorTitle(flavorTitle), flavorText(flavorText) {}
};

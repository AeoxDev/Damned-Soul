#pragma once
#include <string>
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIGameLevelComponent
{
	int value;
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	UIImage image;
	UIText text;
};
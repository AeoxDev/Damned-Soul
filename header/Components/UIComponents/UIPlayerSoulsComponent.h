#pragma once
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIPlayerSoulsComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };

	int value = -1; //Defaulted so we don't have to init to redundant value since it gets set at runtime

	UIImage image;
	UIText text;
};
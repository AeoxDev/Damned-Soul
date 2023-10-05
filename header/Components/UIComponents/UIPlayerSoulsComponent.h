#pragma once
#include <string>
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIPlayerSoulsComponent
{
	float value;
	DirectX::XMFLOAT2 position;
	UIImage image;
	UIText text;
};
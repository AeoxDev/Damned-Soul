#pragma once
#include <string>
#include <DirectXMath.h>
#include <vector>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIPlayerRelicsComponent
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	UIImage baseImage;
	ML_Vector<UIImage> images;
	int imageIndex;

};
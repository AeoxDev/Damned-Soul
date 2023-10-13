#pragma once
#include <DirectXMath.h>
#include <vector>

#include "UIRelicComponent.h"

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIPlayerRelicsComponent
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;
	
	int relicIndex = -1;
	
	UIImage baseImage;
	ML_Vector<UIRelicComponent> relics = {};
	DirectX::XMINT2 gridPosition = { 0, 0 };

};
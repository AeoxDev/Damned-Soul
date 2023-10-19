#pragma once
#include <DirectXMath.h>

#include "UIRelicComponent.h"

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIPlayerRelicsComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };
	
	int relicIndex = -1;
	
	UIImage baseImage;
	ML_Vector<UIRelicComponent> relics = {}; //TODO remove and place in registry
	DirectX::XMINT2 gridPosition = { 0, 0 };

};
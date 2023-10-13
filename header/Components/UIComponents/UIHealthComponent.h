#pragma once
#include <string>
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIHealthComponent
{
	DirectX::XMFLOAT2 position;
	DirectX::XMFLOAT2 scale;

	float value = -1.0f; //Defaulted so we don't have to init to redundant value since it gets set at runtime

	//Don't initialize these when doing addcomponent, call image.Setup() and text.Setup() instead
	UIImage image;
	UIText text;
};
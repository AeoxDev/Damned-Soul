#pragma once
#include "UI\UIImage.h"
#include "UI\UIText.h"

struct UIHealthComponent
{
	DSFLOAT2 position = { 0.0f, 0.0f };
	DSFLOAT2 scale = { 1.0f, 1.0f };

	float value = -1.0f; //Defaulted so we don't have to init to redundant value since it gets set at runtime

	UIImage backgroundImage;
	UIImage healthImage;
	UIText text;
};
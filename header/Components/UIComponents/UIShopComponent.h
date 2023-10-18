#pragma once
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"
#include "UI\UIButton.h"
#include "UIButtonFunctions.h"

#include "MemLib/ML_Vector.hpp"

#include "Registry.h"

struct EntityID;

struct UIShopRelicWindowComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };

	bool doRedraw = true;

	UIText m_relicName;
	UIImage m_baseImage;
	UIImage m_relicImage;

	void Setup(UIText relicName, UIImage baseImage, UIImage relicImage)
	{
		m_relicName = relicName;
		m_baseImage = baseImage;
		m_relicImage = relicImage;
	};
};

struct UIShopComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };

	UIImage baseImage;
	UIText playerInfo;

	UIShopComponent();

	void Setup(EntityID& shop);
};


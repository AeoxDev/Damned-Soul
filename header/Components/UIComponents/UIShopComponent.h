#pragma once
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"
#include "UI\UIButton.h"

#include "MemLib/ML_Vector.hpp"

struct UIShopRelicWindowComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 0.0f, 0.0f };

	ML_Vector<UIButton> m_buttons; //TODO remove and place in registry

	bool doRedraw = true;

	UIText m_relicName;
	UIImage m_baseImage;
	UIImage m_relicImage;

	void Setup(ML_Vector<UIButton> buttons, UIText relicName, UIImage baseImage, UIImage relicImage)
	{
		m_buttons = buttons;
		m_relicName = relicName;
		m_baseImage = baseImage;
		m_relicImage = relicImage;
	};
};

struct UIShopComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 0.0f, 0.0f };

	UIImage baseImage;
	UIText playerInfo;

	UIButton rerollButton;

	ML_Vector<UIShopRelicWindowComponent> relicWindows; //TODO remove and place in registry

	UIShopComponent(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, UIImage baseImage, UIText playerInfo, UIButton rerollButton, ML_Vector<UIShopRelicWindowComponent> relicWindows)
		: position(position), scale(scale), baseImage(baseImage), playerInfo(playerInfo), rerollButton(rerollButton), relicWindows(relicWindows) {};
};

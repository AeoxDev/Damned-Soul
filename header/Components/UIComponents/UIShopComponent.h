#pragma once
#include <DirectXMath.h>

#include "UI\UIImage.h"
#include "UI\UIText.h"
#include "UI\UIButton.h"
#include "UIButtonFunctions.h"
#include "RelicFunctions.h"

#include "MemLib/ML_Vector.hpp"
#include "MemLib/ML_Array.hpp"
#include "MemLib/ML_String.hpp"

#include "Registry.h"

struct EntityID;

struct UIShopRelicWindowComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };

	bool doRedraw = true;

	bool locked = false;

	UIImage m_baseImage;

	//TODO change to using UIRelicComponent in the ecs instead
	UIText m_relicName;
	UIImage m_relicImage;

	void Setup(UIImage baseImage, UIText relicName, UIImage relicImage)
	{
		m_baseImage = baseImage;

		m_relicName = relicName;
		m_relicImage = relicImage;
	};
};

struct UIShopComponent
{
	DirectX::XMFLOAT2 position = { 0.0f, 0.0f };
	DirectX::XMFLOAT2 scale = { 1.0f, 1.0f };

	UIImage baseImage;
	UIText playerInfo;

	ML_Vector<Relics::RelicMetaData> relics;

	UIShopComponent();

	void Setup();
};


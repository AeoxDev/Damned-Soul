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


	void Setup(UIImage baseImage)
	{
		m_baseImage = baseImage;
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


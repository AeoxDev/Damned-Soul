#pragma once
#include "UIComponent.h"

#include "MemLib/ML_String.hpp"

struct UIText
{
	UIComponent m_UiComponent;
	std::wstring m_Text;

	UIText() = default;
	~UIText() = default;

	void Setup(const ML_String& text, DirectX::XMFLOAT2 position = { 0.0f, 0.0f },
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	void Draw();
	void UpdateText(const ML_String text, bool ignoreScale = false);
};


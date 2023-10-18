#pragma once
#include <string>
#include "UIComponent.h"

struct UIText
{
	UIComponent m_UiComponent;
	std::wstring m_Text;

	UIText() = default;
	~UIText() = default;

	void Setup(const std::wstring& string, DirectX::XMFLOAT2 position = { 0.0f, 0.0f },
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	void Draw();
	void UpdateText(std::wstring text);
};


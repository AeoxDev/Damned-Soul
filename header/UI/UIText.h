#pragma once
#include <string>
#include "UIComponent.h"
#include "MemLib/PoolPointer.hpp"

class UIText : public UIComponent
{
private:
	std::wstring m_Text;

public:
	UIText() = default;
	UIText(const std::wstring& string, DirectX::XMFLOAT2 position = { 0.0f, 0.0f },
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	~UIText() = default;

	void Draw();
	void UpdateText(std::wstring text);
};


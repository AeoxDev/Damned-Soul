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
	UIText(PoolPointer<UI>& ui, const std::wstring& string, DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	~UIText() = default;

	void Draw(PoolPointer<UI>& ui);
	void UpdateText(std::wstring text);
};


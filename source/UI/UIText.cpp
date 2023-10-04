#include "UI/UIText.h"

using namespace DirectX;

UIText::UIText(const std::wstring& text, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility)
	:UIComponent(m_Position, m_Scale, m_Rotation, visibility), m_Text(text)
{
	m_Visibility = visibility;
	float fontSize = ui.GetTextFormat()->GetFontSize();

	m_Bounds = { 0, 0, fontSize * m_Text.length(), fontSize + 5};
	SetTransform(position, scale, rotation);
}

void UIText::Draw()
{
	if (m_Visibility)
	{
		ID2D1RenderTarget* rt = ui.GetRenderTarget();

		rt->SetTransform(m_Transform);
		rt->DrawTextW(m_Text.c_str(), (UINT32)m_Text.length(), ui.GetTextFormat(), m_Bounds, ui.GetBrush());
	}
}

void UIText::UpdateText(std::wstring text)
{
	m_Text = text;
	float fontSize = ui.GetTextFormat()->GetFontSize();

	m_Bounds = { 0, 0, fontSize * m_Text.length(), fontSize + 5 };
}

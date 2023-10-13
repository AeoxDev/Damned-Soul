#include "UI/UIText.h"
#include <dwrite.h>
using namespace DirectX;

void UIText::Setup(const std::wstring& string, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, float rotation, bool visibility)
{
	m_Text = string;

	m_UiComponent.Setup(scale, rotation, visibility);
	float fontSize = ui.GetTextFormat()->GetFontSize();

	m_UiComponent.m_OriginalBounds = { 0, 0, fontSize * m_Text.length(), fontSize + 5 };
	m_UiComponent.SetTransform(position, scale, rotation);
}

void UIText::Draw()
{
	if (m_UiComponent.m_Visibility)
	{
		ID2D1RenderTarget* rt = ui.GetRenderTarget();

		rt->SetTransform(m_UiComponent.m_Transform);
		rt->DrawTextW(m_Text.c_str(), (UINT32)m_Text.length(), ui.GetTextFormat(), m_UiComponent.m_CurrentBounds, ui.GetBrush());
	}
}

void UIText::UpdateText(std::wstring text)
{
	m_Text = text;
	float fontSize = ui.GetTextFormat()->GetFontSize();

	m_UiComponent.m_OriginalBounds = { 0, 0, fontSize * m_Text.length(), fontSize + 5 };
}

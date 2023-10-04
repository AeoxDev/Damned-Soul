#include "UI/UIButton.h"

using namespace DirectX;

UIButton::UIButton(const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText, std::function<void()> onClick, std::function<void()> onHover, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	Setup(imageFile, hoverImageFile, buttonText, onClick, onHover, position, scale, rotation, visibility, opacity);
}

void UIButton::Setup(const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText, std::function<void()> onClick, std::function<void()> onHover, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	m_Images[0] = UIImage(imageFile, position, scale, rotation, visibility, opacity);
	m_uiComponent.m_Bounds = m_Images[0].m_Bounds;

	m_onClick = onClick;
	m_onHover = onHover;

	m_uiComponent.SetPosition(position);
	m_uiComponent.SetRotation(rotation);
	m_uiComponent.SetScale(scale);
	m_uiComponent.SetTransform(position, scale, rotation);
	m_uiComponent.SetVisibility(visibility);

	//Hover image is not necessarily needed
	if (hoverImageFile != "")
		m_Images[1] = UIImage(hoverImageFile, position, scale, rotation, visibility, opacity);
	//else
	//	m_Images[1] = UIImage();
	
	//Button text is not necessarily needed
	if (buttonText != L"")
		m_Text = UIText(buttonText, position, scale, rotation, visibility);
	//else
	//	m_Text = UIText();
}

void UIButton::Draw()
{
	if (true == m_uiComponent.m_Visibility)
	{
		m_Images[m_CurrentImage].Draw();
		m_Text.Draw();
	}
}

void UIButton::Interact()
{
	if (m_onClick)
		m_onClick();
}

void UIButton::Hover()
{
	if (m_onHover)
		m_onHover();
}

void UIButton::Release()
{
	if (m_Images[0].GetBitmap() != nullptr)
		m_Images[0].Release();
	if(m_Images[1].GetBitmap() != nullptr)
		m_Images[1].Release();
}

void UIButton::SetPosition(XMFLOAT2 position)
{
	m_uiComponent.SetPosition(position);
	m_Images[0].SetPosition(position);
	m_Images[1].SetPosition(position);
	m_Text.SetPosition(position);
}

void UIButton::SetScale(XMFLOAT2 scale)
{
	m_uiComponent.SetScale(scale);
	m_Images[0].SetScale(scale);
	m_Images[1].SetScale(scale);
	m_Text.SetScale(scale);
}

void UIButton::SetScale(float scale)
{
	m_uiComponent.SetScale(scale);
	m_Images[0].SetScale(scale);
	m_Images[1].SetScale(scale);
	m_Text.SetScale(scale);
}

void UIButton::SetRotation(float rotation)
{
	m_uiComponent.SetRotation(rotation);
	m_Images[0].SetRotation(rotation);
	m_Images[1].SetRotation(rotation);
	m_Text.SetRotation(rotation);
}

void UIButton::SetOpacity(float opacity)
{
	m_Images[0].SetOpacity(opacity);
	m_Images[1].SetOpacity(opacity);
}

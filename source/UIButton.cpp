#include "UIButton.h"

using namespace DirectX;

UIButton::UIButton(PoolPointer<UI>& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText, std::function<void()> onClick, std::function<void()> onHover, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
	:UIComponent(position, scale, rotation, visibility), onClick(onClick), onHover(onHover)
{
	m_Images[0] = UIImage(ui, imageFile, position, scale, rotation, visibility, opacity);

	//Hover image is not necessarily needed
	if (hoverImageFile != "")
		m_Images[1] = UIImage(ui, hoverImageFile, position, scale, rotation, visibility, opacity);
	else
		m_Images[1] = UIImage();
	
	//Button text is not necessarily needed
	if (buttonText != L"")
		m_Text = UIText(ui, buttonText, position, scale, rotation, visibility);
	else
		m_Text = UIText();
}

//UIButton::~UIButton()
//{
//	if (!m_Images.IsNullptr())
//		MemLib::pfree(m_Images);
//}

void UIButton::Draw(PoolPointer<UI>& ui, ID2D1RenderTarget* rt)
{
	if (true == m_Visibility)
	{
		m_Images[m_CurrentImage].Draw(rt);
		m_Text.Draw(ui);
	}
}

//void UIButton::Draw(UI* ui, ID2D1RenderTarget* renderTarget)
//{
//	if (true == m_Visibility)
//	{
//		m_Images[m_CurrentImage].Draw(renderTarget);
//		m_Text.Draw(ui);
//	}
//}

/*void UIButton::Activate()
{
	if (onActive)
		onActive();
}

void UIButton::Deactivate()
{
	if (onDeactive)
			onDeactive();
}*/

void UIButton::Interact()
{
	if (onClick)
		onClick();
}

void UIButton::SetPosition(XMFLOAT2 position)
{
	UIComponent::SetPosition(position);
	m_Images[0].SetPosition(position);
	m_Images[1].SetPosition(position);
	m_Text.SetPosition(position);
}

void UIButton::SetScale(XMFLOAT2 scale)
{
	UIComponent::SetScale(scale);
	m_Images[0].SetScale(scale);
	m_Images[1].SetScale(scale);
	m_Text.SetScale(scale);
}

void UIButton::SetScale(float scale)
{
	UIComponent::SetScale(scale);
	m_Images[0].SetScale(scale);
	m_Images[1].SetScale(scale);
	m_Text.SetScale(scale);
}

void UIButton::SetRotation(float rotation)
{
	UIComponent::SetRotation(rotation);
	m_Images[0].SetRotation(rotation);
	m_Images[1].SetRotation(rotation);
	m_Text.SetRotation(rotation);
}

void UIButton::SetOpacity(float opacity)
{
	m_Images[0].SetOpacity(opacity);
	m_Images[1].SetOpacity(opacity);
}

/*bool UIButton::IsActive() const
{
	return false;
}*/

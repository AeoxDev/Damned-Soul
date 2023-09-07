#include "UIButton.h"

using namespace DirectX::SimpleMath;

UIButton::UIButton(PoolPointer<UI>& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText, std::function<void()> onClick, std::function<void()> onHover, Vector2 position, Vector2 scale, float rotation, bool visibility, float opacity)
	:UIComponent(position, scale, rotation, visibility), onClick(onClick), onHover(onHover)
{
	//function to reset values of the button
	m_Images = MemLib::palloc(sizeof(UIImage)*2);
	*m_Images[0] = UIImage(ui, imageFile, position, scale, rotation, visibility, opacity);
	*m_Images[1] = UIImage(ui, hoverImageFile, position, scale, rotation, visibility, opacity);
	m_Text = UIText(ui, buttonText, position, scale, rotation, visibility);
}

UIButton::~UIButton()
{
	if (!m_Images.IsNullptr())
		MemLib::pfree(m_Images);
}

void UIButton::Draw(PoolPointer<UI>& ui)
{
	if (true == m_Visibility)
	{
		m_Images[m_CurrentImage]->Draw(ui->GetRenderTarget());
		m_Text.Draw(ui);
	}
}

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

void UIButton::SetPosition(DirectX::SimpleMath::Vector2 position)
{
	UIComponent::SetPosition(position);
	m_Images[0]->SetPosition(position);
	m_Images[1]->SetPosition(position);
	m_Text.SetPosition(position);
}

void UIButton::SetScale(DirectX::SimpleMath::Vector2 scale)
{
	UIComponent::SetScale(scale);
	m_Images[0]->SetScale(scale);
	m_Images[1]->SetScale(scale);
	m_Text.SetScale(scale);
}

void UIButton::SetScale(float scale)
{
	UIComponent::SetScale(scale);
	m_Images[0]->SetScale(scale);
	m_Images[1]->SetScale(scale);
	m_Text.SetScale(scale);
}

void UIButton::SetRotation(float rotation)
{
	UIComponent::SetRotation(rotation);
	m_Images[0]->SetRotation(rotation);
	m_Images[1]->SetRotation(rotation);
	m_Text.SetRotation(rotation);
}

void UIButton::SetOpacity(float opacity)
{
	m_Images[0]->SetOpacity(opacity);
	m_Images[1]->SetOpacity(opacity);
}

/*bool UIButton::IsActive() const
{
	return false;
}*/

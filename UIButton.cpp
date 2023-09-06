#include "UIButton.h"

using namespace DirectX::SimpleMath;

UIButton::UIButton(PoolPointer<UI>& ui, const std::string& imageFile, std::wstring buttonText, std::function<void()> onClick, std::function<void()> onHover, Vector2 position, Vector2 scale, float rotation, bool visibility, float opacity)
	:UIComponent(position, scale, rotation, visibility), onClick(onClick), onHover(onHover)
{
	//function to reset values of the button
	m_Image = MemLib::palloc(sizeof(UIImage));
	(UIImage)*m_Image = UIImage(ui, imageFile, position, scale, rotation, visibility, opacity);
	m_Text = UIText(ui, buttonText, position, scale, rotation, visibility);
}

UIButton::~UIButton()
{
	if (!m_Image.IsNullptr())
		MemLib::pfree(m_Image);
}

void UIButton::Draw(PoolPointer<UI>& ui)
{
	if (true == m_Visibility)
	{
		m_Image->Draw(ui->GetRenderTarget());
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

}

void UIButton::SetPosition(DirectX::SimpleMath::Vector2 position)
{

}

void UIButton::SetScale(DirectX::SimpleMath::Vector2 scale)
{

}

void UIButton::SetScale(float scale)
{

}

void UIButton::SetRotation(float rotation)
{

}

void UIButton::SetOpacity(float opacity)
{

}

/*bool UIButton::IsActive() const
{
	return false;
}*/

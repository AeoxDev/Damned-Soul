#include "UI/UIComponent.h"
#include "SDLHandler.h"
#include <d2d1.h>

void UIComponent::UpdateTransform()
{
	m_Transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f)
		* D2D1::Matrix3x2F::Translation(m_Position.x, m_Position.y)
		* D2D1::Matrix3x2F::Rotation(m_Rotation, { sdl.WIDTH / 2.0f, sdl.HEIGHT / 2.0f });
}

void UIComponent::SetTransform(DSFLOAT2 position, DSFLOAT2 scale, float rotation)
{
	SetScale(scale);
	SetPosition(position);
	SetRotation(rotation);
}

DSFLOAT2 UIComponent::GetPosition() const
{
	return m_Position;
}

DSFLOAT2 UIComponent::GetScale() const
{
	return m_Scale;
}

float UIComponent::GetRotation() const
{
	return m_Rotation;
}

void UIComponent::Setup(DSFLOAT2 scale, float rotation, bool visibility)
{
	m_Scale = scale;
	m_Rotation = rotation;
	m_Visibility = visibility;
	m_CurrentBounds = { 0, 0, 0, 0 };
	m_OriginalBounds = { 0, 0, 0, 0 };

	m_Position = { 0, 0 };

	UpdateTransform();
}

void UIComponent::SetPosition(DSFLOAT2 position)
{
	// calculate screen space to pixel coords
	// (-1,-1) -> (1,1) => (0,0) -> (width, height)
	// (-1,-1) is the bottom left coorner, (1, 1) is the top right coorner
	DSFLOAT2 pixelCoords = { (position.x + 1.0f) * 0.5f * sdl.BASE_WIDTH, (1.0f - position.y) * 0.5f * sdl.BASE_HEIGHT };

	m_Position = { pixelCoords.x - (m_CurrentBounds.right / 2.0f) , pixelCoords.y - (m_CurrentBounds.bottom / 2.0f)};
	UpdateTransform();
}

void UIComponent::SetScale(DSFLOAT2 scale)
{	
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;
	m_CurrentBounds.right = m_OriginalBounds.right * m_Scale.x;
	m_CurrentBounds.bottom = m_OriginalBounds.bottom * m_Scale.y;

	UpdateTransform();
}

void UIComponent::SetScale(float scale)
{
	m_Scale = { scale, scale };
	m_CurrentBounds.right = m_OriginalBounds.right * m_Scale.x;
	m_CurrentBounds.bottom = m_OriginalBounds.bottom * m_Scale.y;
	UpdateTransform();
}

void UIComponent::SetRotation(float rotation)
{
	m_Rotation = rotation;
	UpdateTransform();
}

void UIComponent::SetVisibility(bool value)
{
	m_Visibility = value;
}

void UIComponent::ToggleVisibility()
{
	m_Visibility = (m_Visibility) ? false : true;
}

bool UIComponent::IsVisible()
{
	return m_Visibility;
}

bool UIComponent::Intersect(DSINT2 mousePos)
{
	return (mousePos.x > m_Position.x) && (mousePos.x < m_Position.x + m_CurrentBounds.right) &&
		(mousePos.y > m_Position.y) && (mousePos.y < m_Position.y + m_CurrentBounds.bottom);
}

#include "UIComponent.h"

using namespace DirectX::SimpleMath;

float scalingFactor = 1.0f;
Vector2 center{ 0.0f, 0.0f };

void UIComponent::UpdateTransform()
{
	m_Transform = D2D1::Matrix3x2F::Scale(m_Scale.x / scalingFactor, m_Scale.y / scalingFactor)
		* D2D1::Matrix3x2F::Translation(m_Position.x, m_Position.y)
		* D2D1::Matrix3x2F::Rotation(m_Rotation, { center.x, center.y });
}

void UIComponent::SetTransform(Vector2 position, Vector2 scale, float rotation)
{
	SetPosition(position);
	SetScale(scale);
	SetRotation(rotation);
}

UIComponent::UIComponent(Vector2 position, Vector2 scale, float rotation, bool visibility)
	:m_Position(position), m_Scale(scale), m_Rotation(rotation), m_Visibility(visibility)
{
	UpdateTransform();
}

Vector2 UIComponent::GetPosition()
{
	return m_Position;
}

Vector2 UIComponent::GetScale()
{
	return m_Scale;
}

float UIComponent::GetRotation()
{
	return m_Rotation;
}

void UIComponent::SetPosition(Vector2 position)
{
	m_Position = { position.x - m_Bounds.right / 2.0f, position.y - m_Bounds.bottom / 2.0f };
	UpdateTransform();
}

void UIComponent::SetScale(Vector2 scale)
{
	m_Scale = scale;
	UpdateTransform();
}

void UIComponent::SetScale(float scale)
{
	m_Scale = { scale, scale };
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

#pragma once
#include "UI.h"
#include "SimpleMath.h"

class UIComponent
{
protected:
	DirectX::SimpleMath::Vector2 m_Position;
	DirectX::SimpleMath::Vector2 m_Scale;
	float m_Rotation;
	D2D1::Matrix3x2F m_Transform;
	D2D1_RECT_F m_Bounds;
	bool m_Visibility;
	void UpdateTransform();
	void SetTransform(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f);

public:
	UIComponent() = default;
	UIComponent(DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	UI& operator=(UI&) = delete;

	DirectX::SimpleMath::Vector2 GetPosition();
	DirectX::SimpleMath::Vector2 GetScale();
	float GetRotation();

	virtual void SetPosition(DirectX::SimpleMath::Vector2 position);
	virtual void SetScale(DirectX::SimpleMath::Vector2 scale);
	virtual void SetScale(float scale);
	virtual void SetRotation(float rotation);
	
	void SetVisibility(bool value);
	void ToggleVisibility();
	bool IsVisible();
};
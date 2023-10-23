#pragma once
#include "UI.h"
#include <DirectXMath.h>
#include <d2d1helper.h>

struct UIComponent
{
	DirectX::XMFLOAT2 m_Position;
	DirectX::XMFLOAT2 m_Scale;
	float m_Rotation;
	D2D1::Matrix3x2F m_Transform;
	D2D1_RECT_F m_CurrentBounds;
	D2D1_RECT_F m_OriginalBounds;
	bool m_Visibility;
	void UpdateTransform();
	void SetTransform(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f);
	UIComponent() = default;
	//UIComponent(DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	UI& operator=(UI&) = delete;

	DirectX::XMFLOAT2 GetPosition();
	DirectX::XMFLOAT2 GetScale();
	float GetRotation();

	//Instead of constructor
	void Setup(DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);

	virtual void SetPosition(DirectX::XMFLOAT2 position);
	virtual void SetScale(DirectX::XMFLOAT2 scale);
	virtual void SetScale(float scale);
	virtual void SetRotation(float rotation);
	
	void SetVisibility(bool value);
	void ToggleVisibility();
	bool IsVisible();
	bool Intersect(DirectX::XMINT2 mousePos);
};
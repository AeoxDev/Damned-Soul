#pragma once
#include "UI.h"
#include <d2d1helper.h>

#include "MemLib/ML_String.hpp"

struct UIComponent
{
	DSFLOAT2 m_Position;
	DSFLOAT2 m_Scale;
	float m_Rotation;
	D2D1::Matrix3x2F m_Transform;
	D2D1_RECT_F m_CurrentBounds;
	D2D1_RECT_F m_OriginalBounds;
	bool m_Visibility;
	void UpdateTransform();
	void SetTransform(DSFLOAT2 position, DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f);
	UIComponent() = default;
	UI& operator=(UI&) = delete;

	DSFLOAT2 GetPosition() const;
	DSFLOAT2 GetScale() const;
	float GetRotation() const;

	//Instead of constructor
	void Setup(DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);

	void SetPosition(DSFLOAT2 position);
	void SetScale(DSFLOAT2 scale);
	void SetScale(float scale);
	void SetRotation(float rotation);
	
	void SetVisibility(bool value);
	void ToggleVisibility();
	bool IsVisible();
	bool Intersect(DSINT2 mousePos);
};
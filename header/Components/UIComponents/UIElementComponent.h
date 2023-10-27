#pragma once
#include "UI.h"

#include "MemLib/ML_String.hpp"

#include <d2d1helper.h>

struct ID2D1Bitmap;

struct UIText
{
	DSBOUNDS m_CurrentBounds;
	DSBOUNDS m_OriginalBounds;
	ML_String m_Text;

	void SetText(const ML_String text);
};


struct UIImage
{
	DSBOUNDS m_CurrentBounds;
	DSBOUNDS m_OriginalBounds;

	ID2D1Bitmap* m_Bitmap = nullptr;

	void SetImage(const ML_String& filepath);
};

struct UIElementComponent
{
	DSFLOAT2 m_Position;
	DSFLOAT2 m_Scale;
	float m_Rotation;

	D2D1::Matrix3x2F m_Transform;

	bool m_doRedraw;
	bool m_Visibility;
	float m_Opacity;

	DSBOUNDS m_CurrentBounds;
	DSBOUNDS m_OriginalBounds;
	ID2D1Bitmap* m_baseBitmap = nullptr;

	UIText m_Text;
	UIImage m_Image;

	void Setup(const ML_String& imageFilepath, const ML_String& text, DSFLOAT2 position = { 0.0f, 0.0f },
		DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void SetPosition(DSFLOAT2 position);
	void SetScale(DSFLOAT2 scale);
	void SetRotation(float rotation);

	void AdjustPosition(DSFLOAT2 values);
	void AdjustScale(DSFLOAT2 values);
	void AdjustRotation(float value);
	
	void SetVisibility(bool value);

	bool Intersect(DSINT2 mousePos);

	void Draw();

	void Release();

	DSFLOAT2 GetPosition() const;
	DSFLOAT2 GetScale() const;
	float GetRotation() const;
	bool GetVisibility() const;

};
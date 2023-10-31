#pragma once
#include "UIComponent.h"

struct ID2D1Bitmap;

struct UIImage
{
	ID2D1Bitmap* m_Bitmap = nullptr;
	UIComponent m_UiComponent;
	float m_Opacity = 1.0f;

	UIImage() = default;
	~UIImage() = default;

	void Setup(const ML_String& filepath, DSFLOAT2 position = { 0.0f, 0.0f }, DSFLOAT2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);
	void Release();
	void Draw();
	void SetOpacity(float opacity);
	ID2D1Bitmap*& GetBitmap();
	void NullifyBitmap();

	void SetImage(const ML_String& filepath);
};

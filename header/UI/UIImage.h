#pragma once
#include "UIComponent.h"

#include "MemLib/ML_String.hpp"

struct ID2D1Bitmap;

struct UIImage
{
	ID2D1Bitmap* m_Bitmap = nullptr;
	UIComponent m_UiComponent;
	float m_Opacity = 1.0f;

	UIImage() = default;
	~UIImage() = default;

	void Setup(const ML_String& filepath, DirectX::XMFLOAT2 position = { 0.0f, 0.0f }, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);
	void Release();
	void Draw();
	void SetOpacity(float opacity);
	ID2D1Bitmap*& GetBitmap();
	void NullifyBitmap();

	void SetImage(const ML_String& filepath);
};

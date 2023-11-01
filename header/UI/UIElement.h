#pragma once
#include "UI.h"

#include "MemLib/ML_String.hpp"

#include <d2d1helper.h>

struct ID2D1Bitmap;

struct UIBase
{
	DSBOUNDS m_CurrentBounds;
	DSBOUNDS m_OriginalBounds;
	D2D1::Matrix3x2F m_Transform;

	DSFLOAT2 m_Position;
	DSFLOAT2 m_PixelCoords;
	DSFLOAT2 m_Scale;
	float m_Rotation;

	bool m_Visibility;
	float m_Opacity;

	void Setup(DSFLOAT2 position = { 0.0f, 0.0f }, DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void UpdateTransform();

	void SetPosition(DSFLOAT2 position);
	void SetScale(DSFLOAT2 scale);
	void SetRotation(float rotation);
	void SetVisibility(bool value);
	void SetOpacity(float opacity);

	DSFLOAT2 GetPosition() const;
	DSFLOAT2 GetPixelCoords() const;
	DSFLOAT2 GetScale() const;
	DSBOUNDS GetBounds() const;
	float GetRotation() const;
	bool GetVisibility() const;
	float GetOpacity() const;
};

struct UIText
{
	UIBase baseUI;
	ML_String m_Text;

	void SetText(const ML_String text);

	void Draw();
};


struct UIImage
{
	UIBase baseUI;

	ML_String m_fileName;
	ID2D1Bitmap* m_Bitmap = nullptr;

	void SetImage(const ML_String& filepath, bool ignoreRename = false);

	void Draw();
};

struct UIComponent
{
	UIImage m_BaseImage;
	UIImage m_Image;
	UIText m_Text;

	void Setup(const ML_String& imageFilepath, const ML_String& baseImageFilepath, const ML_String& text, DSFLOAT2 position = { 0.0f, 0.0f },
		DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void Release();
};
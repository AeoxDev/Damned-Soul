#pragma once
#include "UI.h"

#include "MemLib/ML_String.hpp"
#include "MemLib/ML_Vector.hpp"

#include <d2d1helper.h>

struct ID2D1Bitmap;

struct UIBase
{
	DSBOUNDS m_CurrentBounds = { 0.0f, 0.0f, 0.0f, 0.0f };
	DSBOUNDS m_OriginalBounds = { 0.0f, 0.0f, 0.0f, 0.0f };
	D2D1::Matrix3x2F m_Transform;

	DSFLOAT2 m_Position = {0.0f , 0.0f};
	DSFLOAT2 m_PixelCoords = { 0.0f , 0.0f };
	DSFLOAT2 m_Scale = { 1.0f , 1.0f };
	float m_Rotation = 0.0f;

	bool m_Visibility = true;
	float m_Opacity = 1.0f;

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
	DSBOUNDS GetOriginalBounds() const;
	float GetRotation() const;
	bool GetVisibility() const;
	float GetOpacity() const;
};

struct UIText
{
	UIBase baseUI;
	char* m_Text;

	void SetText(const char* text, DSBOUNDS bounds);

	void Draw();
};

struct UIImage
{
	UIBase baseUI;

	char* m_fileName;
	ID2D1Bitmap* m_Bitmap = nullptr;

	void SetImage(const char* filepath, bool ignoreRename = false);

	void Draw();
};

struct UIComponent
{
	UIImage m_BaseImage;
	UIText m_BaseText;
	ML_Vector<UIImage> m_Images;
	ML_Vector<UIText> m_Texts;

	void Setup(const char* baseImageFilepath, const char* text, DSFLOAT2 position,
		DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void SetAllVisability(bool value);

	void AddImage(const char* imageFilepath, DSFLOAT2 position, DSFLOAT2 scale = { 1.0f, 1.0f }, bool translateText = true);

	void AddText(const char* text, DSBOUNDS textBounds, DSFLOAT2 position, DSFLOAT2 scale = { 1.0f, 1.0f });

	void Release();
};
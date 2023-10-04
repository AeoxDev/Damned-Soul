#pragma once
#include <string>
#include "UIComponent.h"
#include "MemLib/PoolPointer.hpp"

class UIImage : public UIComponent
{
private:
	ID2D1Bitmap* m_Bitmap = nullptr;
	float m_Opacity = 1.0f;

public:
	UIImage() = default;
	UIImage(const std::string& file, DirectX::XMFLOAT2 position = { 0.0f, 0.0f }, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visbility = true, float opacity = 1.0f);
	~UIImage() = default;
	void Release();
	void Draw();
	void SetOpacity(float opacity);
	ID2D1Bitmap*& GetBitmap();
	void NullifyBitmap();
};

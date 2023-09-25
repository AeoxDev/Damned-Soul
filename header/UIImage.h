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
	UIImage(UI& ui, const std::string& file, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visbility = true, float opacity = 1.0f);
	//UIImage(UI* ui, const std::string& file, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f },
	//	float rotation = 0.0f, bool visbility = true, float opacity = 1.0f);
	//UIImage(ID2D1Bitmap* bitmap, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f },
	//	float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);
	~UIImage();
	void Release();
	void Draw(ID2D1RenderTarget* renderTarget);
	void SetOpacity(float opacity);
	ID2D1Bitmap*& GetBitmap();
	void NullifyBitmap();
};

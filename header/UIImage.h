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
	UIImage(PoolPointer<UI>& ui, const std::string& file, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visbility = true, float opacity = 1.0f);
	UIImage(UI* ui, const std::string& file, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visbility = true, float opacity = 1.0f);
	UIImage(ID2D1Bitmap* bitmap, DirectX::SimpleMath::Vector2 position, DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f },
		float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);
	~UIImage();
	void Draw(ID2D1RenderTarget* renderTarget);
	void SetOpacity(float opacity);
	ID2D1Bitmap*& GetBitmap();
	void NullifyBitmap();
};

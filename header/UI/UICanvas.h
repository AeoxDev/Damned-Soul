#pragma once
#include <map>
#include <vector>
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"
#include "MemLib\ML_Vector.hpp"
#include "MemLib\ML_Map.hpp"
#include <DirectXMath.h>

#define UI_CANVAS_HEADER 0b10101010101010101010101

struct UICanvas
{
	int header = 0b10101010101010101010101;
	bool m_Exit = false;

	ML_Vector<UIButton> m_Buttons;
	ML_Map<const std::string, UIImage> m_Images;
	ML_Map<const std::string, UIText> m_Texts;

	void DrawButtons(UI& ui, ID2D1RenderTarget*& renderTarget);
	void DrawImages(ID2D1RenderTarget*& renderTarget);
	void DrawTexts(UI& ui);

	void AddButton(UI& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonString, std::function<void()> onClick,
		std::function<void()> onHover, /*std::function<void()> onClick, std::function<void()> onClick,*/ DirectX::XMFLOAT2 position,/* bool isActive = false,*/
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void AddImage(UI& ui, const std::string& name, const std::string& file, DirectX::XMFLOAT2 position,
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void AddText(UI& ui, const std::string& name, const std::wstring& text, DirectX::XMFLOAT2 position,
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	UICanvas();
	void Setup();
	~UICanvas();
	void Release();

	//void Resize() = 0;
	void Reset();
	void Render(UI& ui);
	void Update() {};
	
	bool Exit();
};
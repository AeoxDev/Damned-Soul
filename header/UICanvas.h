#pragma once
#include <map>
#include <vector>
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"
//#include "PoolUtility.hpp"
#include "MemLib\ML_Vector.hpp"
#include "MemLib\ML_Map.hpp"
#include <DirectXMath.h>

struct UICanvas
{
	bool m_Exit = false;

	ML_Vector<UIButton> m_Buttons;
	//std::map<const std::string, PoolPointer<UIImage>> m_Images;
	ML_Map<const std::string, UIImage> m_Images;
	//std::map<const std::string, UIImage*> m_Images;
	ML_Map<const std::string, UIText> m_Texts;
	//std::map<const std::string, UIText> m_Texts;

	void DrawButtons(UI& ui, ID2D1RenderTarget* renderTarget);
	void DrawImages(ID2D1RenderTarget* renderTarget);
	void DrawTexts(UI& ui);

	/*void DrawButtons(UI* ui, ID2D1RenderTarget* renderTarget);
	void DrawImages(ID2D1RenderTarget* renderTarget);
	void DrawTexts(UI* ui);*/

	void AddButton(UI& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonString, std::function<void()> onClick,
		std::function<void()> onHover, /*std::function<void()> onClick, std::function<void()> onClick,*/ DirectX::XMFLOAT2 position,/* bool isActive = false,*/
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void AddImage(UI& ui, const std::string& name, const std::string& file, DirectX::XMFLOAT2 position,
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void AddText(UI& ui, const std::string& name, const std::wstring& text, DirectX::XMFLOAT2 position,
		DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);
	UICanvas();
	void Setup();
	virtual ~UICanvas();
	virtual void Release();

	//virtual void Resize() = 0;
	virtual void Reset();
	//virtual void Render(PoolPointer<UI>& ui);
	virtual void Render(UI& ui);
	//virtual void Render(UI* ui);
	virtual void Update() {};
	
	bool Exit();
};

//inline bool UICanvas::m_KeyDown[] = { false };
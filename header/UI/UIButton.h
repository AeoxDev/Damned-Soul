#pragma once
#include <functional>
#include "UIText.h"
#include "UIImage.h"
#include "MemLib/MemLib.hpp"
#include <DirectXMath.h>

// Expected to be saved in a pool pointer
struct UIButton 
{
	UIComponent m_uiComponent;
	std::function<void()> m_onClick;			//On click function for a button
	std::function<void()> m_onHover;			//On hover function for a button
	
	unsigned int m_CurrentImage = 0;
	UIImage m_Images[2];					//2 Images, button image and hover button image (can be null)
	UIText m_Text;							//The text on the button

	UIButton() = default;
	UIButton(const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText,
		std::function<void()> onClick, std::function<void()> onHover, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f,
		bool visibility = true, float opacity = 1.0f);

	void Setup(const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText,
		std::function<void()> onClick, std::function<void()> onHover, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f,
		bool visibility = true, float opacity = 1.0f);

	void Draw();

	void Interact();
	void Hover();
	void Release();

	void SetHover(int);

	void SetPosition(DirectX::XMFLOAT2 position);
	void SetScale(DirectX::XMFLOAT2 scale);
	void SetScale(float scale);
	void SetRotation(float rotation);
	void SetOpacity(float opacity);
};
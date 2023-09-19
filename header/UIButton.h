#pragma once
#include <functional>
#include "UIText.h"
#include "UIImage.h"
#include "MemLib/MemLib.hpp"
#include <DirectXMath.h>

// Expected to be saved in a pool pointer
class UIButton : public UIComponent
{
private:
	std::function<void()> onClick;			//On click function for a button
	std::function<void()> onHover;			//On hover function for a button
	//std::function<void()> onActive;		//incase we need for controllers
	//std::function<void()> onDeactive;		//incase we need for controllers
	
	unsigned int m_CurrentImage = 0;
	UIImage m_Images[2];					//2 Images, button image and hover button image (can be null)
	UIText m_Text;							//The text on the button

	//bool isActive;						//bool for checking if a Controller is "hovering" a button
public:
	UIButton() = default;
	UIButton(PoolPointer<UI>& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonText,
		std::function<void()> onClick, std::function<void()> onHover, /* std::function<void()> onActive, std::function<void()> onDeactive, */
		DirectX::XMFLOAT2 position, /*bool isActive = false, */ DirectX::XMFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f,
		bool visibility = true, float opacity = 1.0f);
	//~UIButton();

	void Draw(PoolPointer<UI>& ui, ID2D1RenderTarget* renderTarget);
	//void Draw(UI* ui, ID2D1RenderTarget* renderTarget);

	void Activate();
	void Deactivate();
	void Interact();
	void Release();

	void SetPosition(DirectX::XMFLOAT2 position);
	void SetScale(DirectX::XMFLOAT2 scale);
	void SetScale(float scale);
	void SetRotation(float rotation);
	void SetOpacity(float opacity);
	//bool IsActive() const;				//Controller stuff
};
#pragma once
#include <functional>
#include "UIText.h"
#include "UIImage.h"
#include "MemLib/MemLib.hpp"

class UIButton : public UIComponent
{
private:
	std::function<void()> onClick;			//On click function for a button
	std::function<void()> onHover;			//On hover function for a button
	//std::function<void()> onActive;		//incase we need for controllers
	//std::function<void()> onDeactive;		//incase we need for controllers
	
	PoolPointer<UIImage> m_Image;
	UIText m_Text;

	//bool isActive;						//bool for checking if a Controller is "hovering" a button
public:
	UIButton() = default;
	UIButton(PoolPointer<UI>& ui, const std::string& imageFile, std::wstring buttonText,
		std::function<void()> onClick, std::function<void()> onHover, /* std::function<void()> onActive, std::function<void()> onDeactive, */
		DirectX::SimpleMath::Vector2 position, /*bool isActive = false, */ DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f,
		bool visibility = true, float opacity = 1.0f);
	~UIButton();

	void Draw(PoolPointer<UI>& ui);
	void Activate();
	void Deactivate();
	void Interact();

	void SetPosition(DirectX::SimpleMath::Vector2 position);
	void SetScale(DirectX::SimpleMath::Vector2 scale);
	void SetScale(float scale);
	void SetRotation(float rotation);
	void SetOpacity(float opacity);
	//bool IsActive() const;				//Controller stuff
};
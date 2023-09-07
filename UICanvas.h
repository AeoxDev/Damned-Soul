#pragma once
#include <map>
#include <vector>
#include "UIButton.h"
#include "UIImage.h"
#include "UIText.h"

class UICanvas
{
protected:
	bool m_Exit = false;

	std::vector<PoolPointer<UIButton>> m_Buttons;
	std::map<std::string, PoolPointer<UIImage>> m_Images;
	std::map<std::string, UIText> m_Texts;

	void DrawButtons(PoolPointer<UI>& ui);
	void DrawImages(PoolPointer<UI>& ui);
	void DrawTexts(PoolPointer<UI>& ui);

	void AddButton(PoolPointer<UI>& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonString, std::function<void()> onClick,
		std::function<void()> onHover, /*std::function<void()> onClick, std::function<void()> onClick,*/ DirectX::SimpleMath::Vector2 position,/* bool isActive = false,*/
		DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);

	void AddImage(PoolPointer<UI>& ui, const std::string& name, const std::string& file, DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f);
	
	void AddText(PoolPointer<UI>& ui, const std::string& name, const std::wstring& text, DirectX::SimpleMath::Vector2 position,
		DirectX::SimpleMath::Vector2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true);

	static bool m_KeyDown[16];
public:
	UICanvas() = default;
	virtual ~UICanvas();

	virtual void Resize() = 0;
	virtual void Reset();
	virtual void Render(PoolPointer<UI>& ui);
	virtual void Update() {};
	
	bool Exit();
};

inline bool UICanvas::m_KeyDown[] = { false };
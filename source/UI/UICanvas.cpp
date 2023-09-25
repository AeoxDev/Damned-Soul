#include "UICanvas.h"

using namespace DirectX;

void UICanvas::DrawButtons(UI& ui, ID2D1RenderTarget* rt)
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		m_Buttons[i].Draw(ui, rt);
}

void UICanvas::DrawImages(ID2D1RenderTarget* rt)
{
	for (auto& [name, image] : m_Images)
		image.Draw(rt);
}

void UICanvas::DrawTexts(UI& ui)
{
	for (auto& [name, text] : m_Texts)
		text.Draw(ui);
}

void UICanvas::AddButton(UI& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonString, std::function<void()> onClick, std::function<void()> onHover, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	m_Buttons.push_back(UIButton());
	m_Buttons[m_Buttons.size() - 1].Setup(ui, imageFile, hoverImageFile, buttonString, onClick, onHover, position, scale, rotation, visibility, opacity);
}

void UICanvas::AddImage(UI& ui, const std::string& name, const std::string& file, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	UIImage temp = UIImage(ui, file, position, scale, rotation, visibility, opacity);
	m_Images.emplace(name, temp);
}

void UICanvas::AddText(UI& ui, const std::string& name, const std::wstring& text, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility)
{
	UIText temp = UIText(ui, text, position, scale, rotation, visibility);
	m_Texts.emplace(name, temp);
}

UICanvas::UICanvas()
{
	Setup();
}

void UICanvas::Setup()
{
	m_Exit = false;
	m_Buttons = ML_Vector<UIButton>();
	m_Images = ML_Map<const std::string, UIImage>();
	m_Texts = ML_Map<const std::string, UIText>();
}

UICanvas::~UICanvas()
{
}

void UICanvas::Release()
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i].Release();
	}
	for (auto& [key, image] : m_Images)
		image.Release();
}

void UICanvas::Reset()
{
	m_Exit = false;
}

void UICanvas::Render(UI& ui)
{
	ID2D1RenderTarget* rt = ui.GetRenderTarget();
	DrawButtons(ui, rt);
	DrawImages(rt);
	DrawTexts(ui);
}

bool UICanvas::Exit()
{
	return m_Exit;
}

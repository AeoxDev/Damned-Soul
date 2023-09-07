#include "UICanvas.h"

using namespace DirectX::SimpleMath;

void UICanvas::DrawButtons(PoolPointer<UI>& ui)
{
	for (auto& button : m_Buttons)
		button->Draw(ui);
}

void UICanvas::DrawImages(PoolPointer<UI>& ui)
{
	ID2D1RenderTarget* rt = ui->GetRenderTarget();
	for (auto& [name, image] : m_Images)
		image->Draw(rt);
}

void UICanvas::DrawTexts(PoolPointer<UI>& ui)
{
	for (auto& [name, text] : m_Texts)
		text.Draw(ui);
}

void UICanvas::AddButton(PoolPointer<UI>& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonString, std::function<void()> onClick, std::function<void()> onHover, Vector2 position, Vector2 scale, float rotation, bool visibility, float opacity)
{
	PoolPointer<UIButton> temp = MemLib::palloc(sizeof(UIButton));
	*temp = UIButton(ui, imageFile, hoverImageFile, buttonString, onClick, onHover, position, scale, rotation, visibility, opacity);
	m_Buttons.emplace_back(temp);
	
}

void UICanvas::AddImage(PoolPointer<UI>& ui, const std::string& name, const std::string& file, Vector2 position, Vector2 scale, float rotation, bool visibility, float opacity)
{
	PoolPointer<UIImage> temp = MemLib::palloc(sizeof(UIImage));
	*temp = UIImage(ui, file, position, scale, rotation, visibility, opacity);
	m_Images.emplace(name, temp);
}

void UICanvas::AddText(PoolPointer<UI>& ui, const std::string& name, const std::wstring& text, Vector2 position, Vector2 scale, float rotation, bool visibility)
{
	m_Texts.emplace(name, UIText(ui, text, position, scale, rotation, visibility));
}

UICanvas::~UICanvas()
{
	for (auto& [name, image] : m_Images)
		MemLib::pfree(image);

	for (auto& button : m_Buttons)
		MemLib::pfree(button);
}

void UICanvas::Reset()
{
	m_Exit = false;
}

void UICanvas::Render(PoolPointer<UI>& ui)
{
	ui->BeginFrame();

	DrawButtons(ui);
	DrawImages(ui);
	DrawTexts(ui);

	ui->EndFrame();
}

bool UICanvas::Exit()
{
	return m_Exit;
}

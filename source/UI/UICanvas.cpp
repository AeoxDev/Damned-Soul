#include "UICanvas.h"

using namespace DirectX;

void UICanvas::DrawButtons(UI& ui, ID2D1RenderTarget*& rt)
{
	/*for (auto& button : m_Buttons)
		button->Draw(ui, rt);*/

	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
		m_Buttons[i].Draw(ui, rt);
}

void UICanvas::DrawImages(ID2D1RenderTarget*& rt)
{
	for (auto& [name, image] : m_Images)
		image.Draw(rt);
	//auto keys = m_Images.GetKeys();
	//for (unsigned int i = 0; i < m_Images.Size(); ++i)
	//	m_Images[keys[i]].Draw(rt);
}

void UICanvas::DrawTexts(UI& ui)
{
	for (auto& [name, text] : m_Texts)
		text.Draw(ui);
	//auto keys = m_Texts.GetKeys();
	//for (unsigned int i = 0; i < m_Texts.Size(); ++i)
	//	m_Texts[keys[i]].Draw(ui);
}

void UICanvas::AddButton(UI& ui, const std::string& imageFile, const std::string& hoverImageFile, std::wstring buttonString, std::function<void()> onClick, std::function<void()> onHover, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	//PoolPointer<UIButton> temp = MemLib::palloc(sizeof(UIButton));
	//UIButton temp(ui, imageFile, hoverImageFile, buttonString, onClick, onHover, position, scale, rotation, visibility, opacity);
	//m_Buttons.push_back(ui, imageFile, hoverImageFile, buttonString, onClick, onHover, position, scale, rotation, visibility, opacity);//(temp);
	m_Buttons.push_back(UIButton());
	m_Buttons[m_Buttons.size() - 1].Setup(ui, imageFile, hoverImageFile, buttonString, onClick, onHover, position, scale, rotation, visibility, opacity);
}

void UICanvas::AddImage(UI& ui, const std::string& name, const std::string& file, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	//PoolPointer<UIImage> temp = MemLib::palloc(sizeof(UIImage));
	//*temp = UIImage(ui, file, position, scale, rotation, visibility, opacity);
	UIImage temp = UIImage(ui, file, position, scale, rotation, visibility, opacity);
	m_Images.emplace(name, temp);
}

//void UICanvas::AddImage(UI* ui, const std::string& name, const std::string& file, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
//{
	/*PoolPointer<UIImage> temp = MemLib::palloc(sizeof(UIImage));
	*temp = UIImage(ui, file, position, scale, rotation, visibility, opacity);
	m_Images.emplace(name, temp);*/

	//m_Images.emplace(name, new UIImage(ui, file, position, scale, rotation, visibility, opacity));
//}

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
	//for (auto& [name, image] : m_Images)
	//	if (false == image.IsNullptr())
	//		MemLib::pfree(image);
	//MemLib::pfree(image);

	/*for (auto& button : m_Buttons)
		if (false == button.IsNullptr())
			MemLib::pfree(button);*/
}

void UICanvas::Release()
{
	for (unsigned int i = 0; i < m_Buttons.size(); ++i)
	{
		m_Buttons[i].Release();
	}
	for (auto& [key, value] : m_Images)
		value.Release();
	//for (unsigned int i = 0; i < m_Images.Size(); ++i)
	//{
	//	m_Images.m_values[i].Release();
	//}
	//for (auto& [key, value] : m_Texts)
	//	value.Release();
	//for (unsigned int i = 0; i < m_Texts.Size(); ++i)
	//{
	//	m_Texts.m_values[i].Release();
	//}
}

void UICanvas::Reset()
{
	m_Exit = false;
}

//void UICanvas::Render(PoolPointer<UI>& ui)
//{
//	ID2D1RenderTarget* rt = ui->GetRenderTarget();
//	DrawButtons(ui, rt);
//	DrawImages(rt);
//	DrawTexts(ui);
//}

void UICanvas::Render(UI& ui)
{
	ID2D1RenderTarget* rt = ui.GetRenderTarget();
	DrawButtons(ui, rt);
	DrawImages(rt);
	DrawTexts(ui);
	//rt->Release();
}


//void UICanvas::Render(UI* ui)
//{
//	ID2D1RenderTarget* rt = ui->GetRenderTarget();
//	DrawButtons(ui, rt);
//	DrawImages(rt);
//	DrawTexts(ui);
//}

bool UICanvas::Exit()
{
	return m_Exit;
}

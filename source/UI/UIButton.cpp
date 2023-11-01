//#include "UI/UIButton.h"
//
//void UIButton::Setup(const ML_String& imageFile, const ML_String& hoverImageFile, ML_String buttonText, void* onClick, DSFLOAT2 position, DSFLOAT2 scale, float rotation, bool visibility, float opacity)
//{
//	m_UiComponent.Setup(scale, rotation, visibility);
//	m_Images[0].Setup(imageFile, position, scale, rotation, visibility, opacity);
//	m_UiComponent.m_OriginalBounds = m_Images[0].m_UiComponent.m_OriginalBounds;
//
//	m_onClick = onClick;
//
//	m_doRedraw = true;
//
//	shopPosition = 0;
//
//	m_CurrentImage = 0;
//
//	//Don't we do this already in constructor etc?
//	m_UiComponent.SetScale(scale);
//	m_UiComponent.SetPosition(position);
//	m_UiComponent.SetRotation(rotation);
//	m_UiComponent.SetTransform(position, scale, rotation);
//	m_UiComponent.SetVisibility(visibility);
//
//	//Hover image is not necessarily needed
//	if (hoverImageFile != "")
//		m_Images[1].Setup(hoverImageFile, position, scale, rotation, visibility, opacity);
//
//	//Button text is not necessarily needed
//	if (buttonText != "")
//		m_Text.Setup(buttonText, position, scale, rotation, visibility);
//}
//
//void UIButton::Draw()
//{
//	if (m_UiComponent.m_Visibility)
//	{
//		m_Images[m_CurrentImage].Draw();
//		m_Text.Draw();
//		m_CurrentImage = 0;
//	}
//}
//
//void UIButton::Interact(void* args)
//{
//	if (m_onClick && m_UiComponent.m_Visibility)
//		((void(*)(void*))m_onClick)(args);
//}
//
//void UIButton::Hover()
//{
//	if (/*m_onHover && */m_UiComponent.m_Visibility)
//	{
//		m_CurrentImage = 1;
//		//m_onHover();
//	}
//}
//
//void UIButton::Release()
//{
//	if (m_Images[0].GetBitmap() != nullptr)
//		m_Images[0].Release();
//	if(m_Images[1].GetBitmap() != nullptr)
//		m_Images[1].Release();
//}
//
//void UIButton::SetPosition(DSFLOAT2 position)
//{
//	m_UiComponent.SetPosition(position);
//	m_Images[0].m_UiComponent.SetPosition(position);
//	m_Images[1].m_UiComponent.SetPosition(position);
//	m_Text.m_UiComponent.SetPosition(position);
//}
//
//void UIButton::SetScale(DSFLOAT2 scale)
//{
//	m_UiComponent.SetScale(scale);
//	m_Images[0].m_UiComponent.SetScale(scale);
//	m_Images[1].m_UiComponent.SetScale(scale);
//	m_Text.m_UiComponent.SetScale(scale);
//}
//
//void UIButton::SetScale(float scale)
//{
//	m_UiComponent.SetScale(scale);
//	m_Images[0].m_UiComponent.SetScale(scale);
//	m_Images[1].m_UiComponent.SetScale(scale);
//	m_Text.m_UiComponent.SetScale(scale);
//}
//
//void UIButton::SetRotation(float rotation)
//{
//	m_UiComponent.SetRotation(rotation);
//	m_Images[0].m_UiComponent.SetRotation(rotation);
//	m_Images[1].m_UiComponent.SetRotation(rotation);
//	m_Text.m_UiComponent.SetRotation(rotation);
//}
//
//void UIButton::SetOpacity(float opacity)
//{
//	m_Images[0].SetOpacity(opacity);
//	m_Images[1].SetOpacity(opacity);
//}

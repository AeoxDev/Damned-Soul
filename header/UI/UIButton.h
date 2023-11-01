#pragma once
//#include <functional>
//#include "UIText.h"
//#include "UIImage.h"
//#include "MemLib/MemLib.hpp"
//
//// Expected to be saved in a pool pointer
//struct UIButton 
//{
//	UIComponent m_UiComponent;
//	void* m_onClick;
//	bool m_doRedraw = true;
//	int shopPosition = 0;
//	
//	unsigned int m_CurrentImage = 0;
//	UIImage m_Images[2];					//2 Images, button image and hover button image (can be null)
//	UIText m_Text;							//The text on the button
//
//	UIButton() = default;
//
//	void Setup(const ML_String& imageFile, const ML_String& hoverImageFile, ML_String buttonText, void* onClick,
//		DSFLOAT2 position = { 0.0f, 0.0f }, DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f,
//		bool visibility = true, float opacity = 1.0f);
//
//	void Draw();
//
//	void Interact(void* args);
//	void Hover();
//	void Release();
//
//	//void SetHover(int);
//
//	void SetPosition(DSFLOAT2 position);
//	void SetScale(DSFLOAT2 scale);
//	void SetScale(float scale);
//	void SetRotation(float rotation);
//	void SetOpacity(float opacity);
//};
#pragma once
#include <d2d1.h>
#include <wincodec.h>
#include <dwrite.h>



class UI
{
private:
	ID2D1Factory* m_Factory = nullptr;					//Used to create other Direct2D resources that you can use to draw or describe shapes. (basicly device)
	ID2D1RenderTarget* m_RenderTarget = nullptr;		//Represents an object that can receive drawing command
	IDWriteFactory* m_WriteFactory = nullptr;			//Used to create all subsequent DirectWrite objects
	IWICImagingFactory* m_ImagingFactory = nullptr;		//Exposes methods used to create components for the Windows Imaging Component (WIC) such as decoders, encoders and pixel format converters.
	IDWriteTextFormat* m_TextFormat = nullptr;			//Describes the font and paragraph properties used to format text, and it describes locale information.
	ID2D1SolidColorBrush* m_Brush = nullptr;			//Paints an area with a solid color.
	ID2D1SolidColorBrush* m_YellowBrush = nullptr;		//Another brush with a diffrent color, might remake in order to more eaisly make more brushes later if needed :)


public:
	UI();
	~UI();

	void BeginFrame();
	void EndFrame();

	void ResizeRelease();
	void Resize();

	ID2D1Factory*& GetFactory();
	ID2D1RenderTarget*& GetRenderTarget();
	IDWriteFactory*& GetWriteFactory();
	IWICImagingFactory*& GetImagingFactory();
	IDWriteTextFormat*& GetTextFormat();
	ID2D1SolidColorBrush*& GetBrush();
	ID2D1SolidColorBrush*& GetYellowBrush();
};


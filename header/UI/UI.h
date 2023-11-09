#pragma once

struct ID2D1Factory;
struct ID2D1RenderTarget;
struct IDWriteFactory;
struct IWICImagingFactory;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

struct UI;

extern UI ui;

struct UI
{
	ID2D1Factory* m_Factory = nullptr;					//Used to create other Direct2D resources that you can use to draw or describe shapes. (basicly device)
	ID2D1RenderTarget* m_RenderTarget = nullptr;		//Represents an object that can receive drawing command
	IDWriteFactory* m_WriteFactory = nullptr;			//Used to create all subsequent DirectWrite objects
	IWICImagingFactory* m_ImagingFactory = nullptr;		//Exposes methods used to create components for the Windows Imaging Component (WIC) such as decoders, encoders and pixel format converters.
	IDWriteTextFormat* m_TextFormat = nullptr;			//Describes the font and paragraph properties used to format text, and it describes locale information.
	ID2D1SolidColorBrush* m_Brush = nullptr;			//Paints an area with a solid color.
	ID2D1SolidColorBrush* m_YellowBrush = nullptr;		//Another brush with a diffrent color, might remake in order to more eaisly make more brushes later if needed :)
	int RenderSlot = -1;

	void BeginFrame();
	void EndFrame();

	UI();
	void Setup();
	void Release();

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

struct DSFLOAT2
{
	float x;
	float y;
};

struct DSINT2
{
	int x;
	int y;
};

struct DSBOUNDS
{
	float left;
	float top;
	float right;
	float bottom;
};
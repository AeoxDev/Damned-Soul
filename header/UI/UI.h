#pragma once

struct ID2D1Factory;
struct ID2D1RenderTarget;
struct IDWriteFactory;
struct IWICImagingFactory;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

struct UI
{
	ID2D1Factory* m_Factory = nullptr;					//Used to create other Direct2D resources that you can use to draw or describe shapes. (basicly device)
	ID2D1RenderTarget* m_RenderTarget = nullptr;		//Represents an object that can receive drawing command
	IDWriteFactory* m_WriteFactory = nullptr;			//Used to create all subsequent DirectWrite objects
	IWICImagingFactory* m_ImagingFactory = nullptr;		//Exposes methods used to create components for the Windows Imaging Component (WIC) such as decoders, encoders and pixel format converters.
	ID2D1SolidColorBrush* m_Brush = nullptr;			//Paints an area with a solid color.
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
	ID2D1SolidColorBrush*& GetBrush();
};

extern UI ui;

//2D float. x, y
struct DSFLOAT2
{
	float x;
	float y;
};

//2D int. x, y
struct DSINT2
{
	int x;
	int y;
};

//Bounds for a UI element. left, top, right, bottom
struct DSBOUNDS
{
	float left;
	float top;
	float right;
	float bottom;
};
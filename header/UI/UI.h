#pragma once

struct ID2D1Factory;
struct ID2D1RenderTarget;
struct IDWriteFactory;
struct IWICImagingFactory;
struct IDWriteTextFormat;
struct ID2D1SolidColorBrush;

enum BrushColor
{
	White,	//This is not black
	Yellow, //Color Kinda Yellow IDK, maybe
	LightSteelBlue, //Color: hot but not heavy; metallic. 69. Last letter in RGB-ish
	Black //And Yellow
};

struct UI
{
	ID2D1Factory* m_Factory = nullptr;					//Used to create other Direct2D resources that you can use to draw or describe shapes. (basicly device)
	ID2D1RenderTarget* m_RenderTarget = nullptr;		//Represents an object that can receive drawing command
	IDWriteFactory* m_WriteFactory = nullptr;			//Used to create all subsequent DirectWrite objects
	IWICImagingFactory* m_ImagingFactory = nullptr;		//Exposes methods used to create components for the Windows Imaging Component (WIC) such as decoders, encoders and pixel format converters.
	ID2D1SolidColorBrush* m_Brush = nullptr;			//Paints an area with a solid color.
	ID2D1SolidColorBrush* m_YellowBrush = nullptr;			//Elliot: Yellow Brush
	ID2D1SolidColorBrush* m_LightSteelBlue = nullptr;			//Elliot: Not that heavy; metallic. 69. Last letter in RGB-ish
	ID2D1SolidColorBrush* m_BlackBrush = nullptr;			//Christian: Black Brush
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
	ID2D1SolidColorBrush*& GetBrush(BrushColor brush = White);
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
#include "UI.h"
#include <iostream>
#include "SDLhandler.h"
#include "D2D1Graphics.h"
#include "UICanvas.h"

UI::UI()
{
	HRESULT hr;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_Factory);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create D2D1Factory" << std::endl;
		return;
	}

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory), reinterpret_cast<IUnknown**>(&m_WriteFactory));
	if (FAILED(hr))
	{
		std::cout << "FAILED to create D2D1WriteFactory" << std::endl;
		return;
	}
	
	UINT DPIX, DPIY;
	DPIX = DPIY = GetDpiForWindow(sdl.window);

	D2D1_RENDER_TARGET_PROPERTIES properties;
	properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN,D2D1_ALPHA_MODE_PREMULTIPLIED), (float)DPIX, (float)DPIY);

	RECT rect;
	GetClientRect(sdl.window, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);

	hr = m_Factory->CreateDxgiSurfaceRenderTarget(UISurface, properties, &m_RenderTarget);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create UI render target" << std::endl;
		return;
	}

	hr = m_WriteFactory->CreateTextFormat(L"Candara", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL,30.0f, L"", &m_TextFormat);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create Text Format" << std::endl;
		return;
	}
	m_TextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);

	hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), &m_Brush);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create standard color brush" << std::endl;
		return;
	}

	hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 230.0f / 255.0f, 0.0f), &m_YellowBrush);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create yellow color brush" << std::endl;
		return;
	}

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&m_ImagingFactory);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create Imaging factory" << std::endl;
		return;
	}
}

UI::~UI()
{
	m_ImagingFactory->Release();
	m_RenderTarget->Release();
	m_WriteFactory->Release();
	m_Brush->Release();
	m_YellowBrush->Release();
	m_Factory->Release();
}

void UI::Render()
{
	BeginFrame();
	if (m_CurrentCanvas != nullptr)
		m_CurrentCanvas->Render(this);


	EndFrame();
}

void UI::BeginFrame()
{
	m_RenderTarget->BeginDraw();
	m_RenderTarget->SetTransform(D2D1::Matrix3x2F::Identity());
}

void UI::EndFrame()
{
	m_RenderTarget->EndDraw();
}

void UI::ResizeRelease()
{
	m_RenderTarget->Release();
}

void UI::Resize()
{
	UINT DPIX, DPIY;
	DPIX = DPIY = GetDpiForWindow(sdl.window);

	D2D1_RENDER_TARGET_PROPERTIES properties;
	properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), (float)DPIX, (float)DPIY);
	RECT rect;
	GetClientRect(sdl.window, &rect);

	HRESULT hr = m_Factory->CreateDxgiSurfaceRenderTarget(UISurface, properties, &m_RenderTarget);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create render target when resizing" << std::endl;
		return;
	}
}

ID2D1Factory*& UI::GetFactory()
{
	return m_Factory;
}

ID2D1RenderTarget*& UI::GetRenderTarget()
{
	return m_RenderTarget;
}

IDWriteFactory*& UI::GetWriteFactory()
{
	return m_WriteFactory;
}

IWICImagingFactory*& UI::GetImagingFactory()
{
	return m_ImagingFactory;
}

IDWriteTextFormat*& UI::GetTextFormat()
{
	return m_TextFormat;
}

ID2D1SolidColorBrush*& UI::GetBrush()
{
	return m_Brush;
}

ID2D1SolidColorBrush*& UI::GetYellowBrush()
{
	return m_YellowBrush;
}

void UI::SetCurrentCanvas(UICanvas* canvas)
{
	m_CurrentCanvas = canvas;
}


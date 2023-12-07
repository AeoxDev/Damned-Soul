#include "SDLhandler.h"
#include "UI/UIRenderer.h"
#include <assert.h>
#include <d2d1.h>

#include <wincodec.h>
#include <dwrite.h>

UI ui;

UI::UI()
{
	
}

void UI::Setup()
{
	HRESULT hr;
	D2D1_FACTORY_OPTIONS options;
	options.debugLevel = D2D1_DEBUG_LEVEL_INFORMATION;
	hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, &m_Factory);
	assert(!FAILED(hr));

	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_WriteFactory), reinterpret_cast<IUnknown**>(&m_WriteFactory));
	assert(!FAILED(hr));
	 
	UINT DPIX, DPIY;
	DPIX = DPIY = GetDpiForWindow(sdl.window);

	D2D1_RENDER_TARGET_PROPERTIES properties;
	properties = D2D1::RenderTargetProperties(D2D1_RENDER_TARGET_TYPE_DEFAULT, D2D1::PixelFormat(DXGI_FORMAT_UNKNOWN, D2D1_ALPHA_MODE_PREMULTIPLIED), (float)DPIX, (float)DPIY);

	RECT rect;
	GetClientRect(sdl.window, &rect);
	D2D1_SIZE_U size = D2D1::SizeU(rect.right - rect.left, rect.bottom - rect.top);

	hr = m_Factory->CreateDxgiSurfaceRenderTarget(UISurface, properties, &m_RenderTarget);
	assert(!FAILED(hr));

	hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(255.0f, 255.0f, 255.0f), &m_Brush);
	assert(!FAILED(hr));
	
	hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 0.225f), &m_YellowBrush);
	assert(!FAILED(hr));
	
	//hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(1.0f, 0.894f, 0.882f), &m_MistyRoseBrush);
	hr = m_RenderTarget->CreateSolidColorBrush(D2D1::ColorF(0.69f, 0.769f, 0.869f), &m_MistyRoseBrush);
	assert(!FAILED(hr));

	hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_IWICImagingFactory, (LPVOID*)&m_ImagingFactory);
	assert(!FAILED(hr));
}

void UI::Release()
{
	m_ImagingFactory->Release();
	m_RenderTarget->Release();
	m_WriteFactory->Release();
	m_Brush->Release();
	m_MistyRoseBrush->Release();
	m_YellowBrush->Release();
	m_Factory->Release();
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
	assert(!FAILED(hr));
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

ID2D1SolidColorBrush*& UI::GetBrush(BrushColor brush)
{
	switch (brush)
	{
	case White:
		return m_Brush;
	case Yellow:
		return m_YellowBrush;
	case MistyRose:
		return m_MistyRoseBrush;
	}
	return m_Brush;
}
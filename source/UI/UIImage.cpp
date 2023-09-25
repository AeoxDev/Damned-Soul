#include "UIImage.h"
#include <iostream>

using namespace DirectX;

UIImage::UIImage(UI& ui, const std::string& file, XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility, float opacity)
	:UIComponent(position, scale, rotation, visibility), m_Bitmap(nullptr), m_Opacity(opacity)
{
	HRESULT hr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* source = nullptr;
	IWICFormatConverter* converter = nullptr;
	IWICImagingFactory* factory = ui.GetImagingFactory();
	const std::wstring path = L"../resource/GUI/" + std::wstring(file.begin(), file.end());

	hr = factory->CreateDecoderFromFilename(path.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create image decoder" << std::endl;
		return;
	}

	hr = decoder->GetFrame(0, &source);
	if (FAILED(hr))
	{
		std::cout << "FAILED to get image source" << std::endl;
		return;
	}

	hr = factory->CreateFormatConverter(&converter);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create image converter" << std::endl;
		return;
	}

	hr = converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
	if (FAILED(hr))
	{
		std::cout << "FAILED to inizialize image source" << std::endl;
		return;
	}

	
	hr = ui.GetRenderTarget()->CreateBitmapFromWicBitmap(converter, NULL, &m_Bitmap);
	if (FAILED(hr))
	{
		std::cout << "FAILED to create image bitmap" << std::endl;
		return;
	}

	m_Bounds = { 0.0f, 0.0f, m_Bitmap->GetSize().width, m_Bitmap->GetSize().height };
	SetTransform(position, scale, rotation);

	converter->Release();
	source->Release();
	decoder->Release();
}

void UIImage::Release()
{
	if (m_Bitmap)
		m_Bitmap->Release();
}

void UIImage::Draw(ID2D1RenderTarget* renderTarget)
{
	if (true == m_Visibility)
	{
		renderTarget->SetTransform(m_Transform);
		renderTarget->DrawBitmap(m_Bitmap, m_Bounds, m_Opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, m_Bounds);
	}
}

void UIImage::SetOpacity(float opacity)
{
	m_Opacity = (opacity > 1) ? 1 : opacity;
	m_Opacity = (opacity < 0) ? 0 : opacity;
	m_Opacity = opacity;
}

ID2D1Bitmap*& UIImage::GetBitmap()
{
	return m_Bitmap;
}

void UIImage::NullifyBitmap()
{
	m_Bitmap = nullptr;
}

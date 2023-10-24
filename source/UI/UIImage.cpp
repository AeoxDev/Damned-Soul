#include "UI/UIImage.h"
#include <d2d1.h>
#include <wincodec.h>


using namespace DirectX;

void UIImage::Setup(const ML_String& filepath, DirectX::XMFLOAT2 position, DirectX::XMFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	m_UiComponent.Setup(scale, rotation, visibility);
	m_Bitmap = nullptr;
	m_Opacity = opacity;

	SetImage(filepath);

	m_UiComponent.m_OriginalBounds = { 0.0f, 0.0f, m_Bitmap->GetSize().width, m_Bitmap->GetSize().height };

	m_UiComponent.SetTransform(position, scale, rotation);

	
}

void UIImage::Release()
{
	if (m_Bitmap)
	{
		m_Bitmap->Release();
		m_Bitmap = nullptr;
	}
}

void UIImage::Draw()
{
	if (m_UiComponent.m_Visibility == true)
	{
		ID2D1RenderTarget* rt = ui.GetRenderTarget();
		rt->SetTransform(m_UiComponent.m_Transform);
		rt->DrawBitmap(m_Bitmap, m_UiComponent.m_CurrentBounds, m_Opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, m_UiComponent.m_CurrentBounds);
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

void UIImage::SetImage(const ML_String& filepath)
{
	Release();

	HRESULT hr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* source = nullptr;
	IWICFormatConverter* converter = nullptr;
	IWICImagingFactory* factory = ui.GetImagingFactory();
	const std::wstring path = L"../resource/GUI/" + std::wstring(filepath.begin(), filepath.end());

	hr = factory->CreateDecoderFromFilename(path.c_str(), NULL, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	assert(!FAILED(hr));

	hr = decoder->GetFrame(0, &source);
	assert(!FAILED(hr));

	hr = factory->CreateFormatConverter(&converter);
	assert(!FAILED(hr));

	hr = converter->Initialize(source, GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, NULL, 0.0f, WICBitmapPaletteTypeMedianCut);
	assert(!FAILED(hr));


	hr = ui.GetRenderTarget()->CreateBitmapFromWicBitmap(converter, NULL, &m_Bitmap);
	assert(!FAILED(hr));

	converter->Release();
	source->Release();
	decoder->Release();
}

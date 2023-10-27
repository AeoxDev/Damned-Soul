#include "UIComponents/UIElementComponent.h"

#include "SDLHandler.h"

#include <wincodec.h>
#include <d2d1.h>

void SetupImage(const ML_String& filepath, ID2D1Bitmap* bitmap) 
{
	if (bitmap)
	{
		bitmap->Release();
		bitmap = nullptr;
	}

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


	hr = ui.GetRenderTarget()->CreateBitmapFromWicBitmap(converter, NULL, &bitmap);
	assert(!FAILED(hr));

	converter->Release();
	source->Release();
	decoder->Release();

}

void SetBounds(D2D1_RECT_F& d2d1Bounds, DSBOUNDS bounds)
{
	d2d1Bounds.left = bounds.left;
	d2d1Bounds.top = bounds.top;
	d2d1Bounds.right = bounds.right;
	d2d1Bounds.bottom = bounds.bottom;
}


void UIText::SetText(const ML_String text)
{
	m_Text = text;
	std::wstring textAsWString(m_Text.begin(), m_Text.end());

	float fontSize = 20;

	m_OriginalBounds = { 0, 0, fontSize * m_Text.length(), fontSize + 5 };
}

void UIImage::SetImage(const ML_String& filepath)
{
	SetupImage(filepath, m_Bitmap);

	m_OriginalBounds = { 0.0f, 0.0f, m_Bitmap->GetSize().width, m_Bitmap->GetSize().height };
}


void UIElementComponent::Setup(const ML_String& imageFilepath, const ML_String& text, DSFLOAT2 position = { 0.0f, 0.0f }, 
	DSFLOAT2 scale = { 1.0f, 1.0f }, float rotation = 0.0f, bool visibility = true, float opacity = 1.0f)
{

	m_Rotation = rotation;
	m_Scale = scale;
	m_Visibility = visibility;
	m_Opacity = opacity;

	m_doRedraw = true;
	
	SetupImage("BaseImage.png", m_baseBitmap);

	m_Text.SetText(text);
	m_Image.SetImage(imageFilepath);


}

void UIElementComponent::SetPosition(DSFLOAT2 position)
{
	DSFLOAT2 pixelCoords = { (position.x + 1.0f) * 0.5f * sdl.BASE_WIDTH, (1.0f - position.y) * 0.5f * sdl.BASE_HEIGHT };

	m_Position = { pixelCoords.x - (m_CurrentBounds.right / 2.0f) , pixelCoords.y - (m_CurrentBounds.bottom / 2.0f) };
}

void UIElementComponent::SetScale(DSFLOAT2 scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;

	m_CurrentBounds.right = m_OriginalBounds.right * m_Scale.x;
	m_CurrentBounds.bottom = m_OriginalBounds.bottom * m_Scale.y;

	m_Text.m_CurrentBounds.right = m_Text.m_OriginalBounds.right * m_Scale.x;
	m_Text.m_CurrentBounds.bottom = m_Text.m_OriginalBounds.right * m_Scale.x;

	m_Image.m_CurrentBounds.right = m_Text.m_OriginalBounds.right * m_Scale.x;
	m_Image.m_CurrentBounds.bottom = m_Text.m_OriginalBounds.right * m_Scale.x;

}

void UIElementComponent::SetRotation(float rotation)
{
	m_Rotation = rotation;
}

void UIElementComponent::SetVisibility(bool value)
{
	m_Visibility = value;
}

bool UIElementComponent::Intersect(DSINT2 mousePos)
{
	return (mousePos.x > m_Position.x) && (mousePos.x < m_Position.x + m_CurrentBounds.right) &&
		(mousePos.y > m_Position.y) && (mousePos.y < m_Position.y + m_CurrentBounds.bottom);
}

void UIElementComponent::Draw()
{
	if (m_Visibility == true)
	{
		ID2D1RenderTarget* rt = ui.GetRenderTarget();
		rt->SetTransform(m_Transform);
		D2D1_RECT_F tempBounds;

		SetBounds(tempBounds, m_CurrentBounds);
		rt->DrawBitmap(m_baseBitmap, tempBounds, m_Opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, tempBounds);
		
		SetBounds(tempBounds, m_Text.m_CurrentBounds);
		std::wstring textAsWString(m_Text.m_Text.begin(), m_Text.m_Text.end());
		rt->DrawTextW(textAsWString.c_str(), (UINT32)textAsWString.length(), ui.GetTextFormat(), tempBounds, ui.GetBrush());

		SetBounds(tempBounds, m_Image.m_CurrentBounds);
		rt->DrawBitmap(m_Image.m_Bitmap, tempBounds, m_Opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, tempBounds);
	}


}

void UIElementComponent::Release()
{
	if (m_baseBitmap)
	{
		m_baseBitmap->Release();
		m_baseBitmap = nullptr;
	}

	if (m_Image.m_Bitmap)
	{
		m_Image.m_Bitmap->Release();
		m_Image.m_Bitmap = nullptr;
	}

}

DSFLOAT2 UIElementComponent::GetPosition() const
{
	return m_Position;
}

DSFLOAT2 UIElementComponent::GetScale() const
{
	return m_Scale;
}

float UIElementComponent::GetRotation() const
{
	return m_Rotation;
}

bool UIElementComponent::GetVisibility() const
{
	return m_Visibility;
}
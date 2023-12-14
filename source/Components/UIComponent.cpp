#include "UIComponents/UIComponent.h"

#include "SDLHandler.h"

#include <wincodec.h>
#include <d2d1.h>

#include <assert.h>

void SetupImage(const char* filepath, ID2D1Bitmap*& bitmap)
{
	if (bitmap)
	{
		bitmap->Release();
		bitmap = nullptr;
	}

	ML_String pathAsString = filepath;

	HRESULT hr;
	IWICBitmapDecoder* decoder = nullptr;
	IWICBitmapFrameDecode* source = nullptr;
	IWICFormatConverter* converter = nullptr;
	IWICImagingFactory* factory = ui.GetImagingFactory();
	const std::wstring path = L"../resource/GUI/" + std::wstring(pathAsString.begin(), pathAsString.end()) + L".png";

#ifdef _DEBUG
	printf("path: %ls\n", path.c_str());
#endif
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

void SetupText(float fontSize, DWRITE_TEXT_ALIGNMENT textAlignment, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, IDWriteTextFormat*& textFormat)
{
	if (textFormat != nullptr)
	{
		textFormat->Release();
		textFormat = nullptr;
	}

	FLOAT fontSizeFactored = fontSize * 1.0f;
	
	ui.GetWriteFactory()->CreateTextFormat(L"Cascadia Code", nullptr, DWRITE_FONT_WEIGHT_NORMAL, DWRITE_FONT_STYLE_NORMAL, DWRITE_FONT_STRETCH_NORMAL, fontSizeFactored, L"", &textFormat);

	textFormat->SetTextAlignment(textAlignment);
	textFormat->SetParagraphAlignment(paragraphAlignment);

}

void SetBounds(D2D1_RECT_F& d2d1Bounds, DSBOUNDS bounds)
{
	d2d1Bounds.left = bounds.left;
	d2d1Bounds.top = bounds.top;
	d2d1Bounds.right = bounds.right;
	d2d1Bounds.bottom = bounds.bottom;
}

void UIBase::Setup(DSFLOAT2 position, DSFLOAT2 scale, float rotation, bool visibility, float opacity)
{
	SetScale(scale);
	SetPosition(position);
	SetRotation(rotation);
	SetVisibility(visibility);
	SetOpacity(opacity);
}

void UIBase::UpdateTransform()
{
	m_Transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f)
		* D2D1::Matrix3x2F::Translation(m_PixelCoords.x, m_PixelCoords.y)
		* D2D1::Matrix3x2F::Rotation(m_Rotation, { sdl.WIDTH / 2.0f, sdl.HEIGHT / 2.0f });
}

void UIBase::SetPosition(DSFLOAT2 position)
{
	DSFLOAT2 pixelCoords = { (position.x + 1.0f) * 0.5f * sdl.BASE_WIDTH, (1.0f - position.y) * 0.5f * sdl.BASE_HEIGHT };

	m_PositionBounds = 
	{ 
		((pixelCoords.x - m_CurrentBounds.right) / (0.5f * sdl.BASE_WIDTH)) - 1.0f ,
		-1 * (((pixelCoords.y - m_CurrentBounds.bottom) - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT)),
		((pixelCoords.x + m_CurrentBounds.right) / (0.5f * sdl.BASE_WIDTH)) - 1.0f,
		-1 * (((pixelCoords.y + m_CurrentBounds.bottom) - (0.5f * sdl.BASE_HEIGHT)) / (0.5f * sdl.BASE_HEIGHT))
	};

	m_PixelCoords = { pixelCoords.x - (m_CurrentBounds.right / 2.0f) , pixelCoords.y - (m_CurrentBounds.bottom / 2.0f) };
	
	m_Position = position;

	UpdateTransform();
}

void UIBase::SetPosition(int pixelX, int pixelY)
{
	m_Position.x = pixelX;
	m_Position.y = pixelY;
	m_PixelCoords.x = pixelX;
	m_PixelCoords.y = pixelY;
	UpdateTransform();
}

void UIBase::SetScale(DSFLOAT2 scale)
{
	m_Scale.x = scale.x;
	m_Scale.y = scale.y;

	m_CurrentBounds.right = m_OriginalBounds.right * m_Scale.x;
	m_CurrentBounds.bottom = m_OriginalBounds.bottom * m_Scale.y;

	UpdateTransform();
}

void UIBase::SetRotation(float rotation)
{
	m_Rotation = rotation;
	UpdateTransform();
}

void UIBase::SetVisibility(bool value)
{
	m_Visibility = value;
}

void UIBase::SetOpacity(float opacity)
{
	m_Opacity = opacity;
}

DSFLOAT2 UIBase::GetPosition() const
{
	return m_Position;
}

DSFLOAT2 UIBase::GetPixelCoords() const
{
	return m_PixelCoords;
}

DSFLOAT2 UIBase::GetScale() const
{
	return m_Scale;
}

DSBOUNDS UIBase::GetBounds() const
{
	return m_CurrentBounds;
}

DSBOUNDS UIBase::GetOriginalBounds() const
{
	return m_OriginalBounds;
}

DSBOUNDS UIBase::GetPositionBounds() const
{
	return m_PositionBounds;
}

float UIBase::GetRotation() const
{
	return m_Rotation;
}

bool UIBase::GetVisibility() const
{
	return m_Visibility;
}

float UIBase::GetOpacity() const
{
	return m_Opacity;
}

void UIText::SetText(const char* text, DSBOUNDS bounds, float fontSize, DWRITE_TEXT_ALIGNMENT textAlignment, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment)
{
	m_Text = text;
	m_fontSize = fontSize;
	m_textAlignment = textAlignment;
	m_paragraphAlignment = paragraphAlignment;

	SetupText(m_fontSize, m_textAlignment, m_paragraphAlignment, m_TextFormat);

	if (bounds.right == 0)
		baseUI.m_OriginalBounds = { 0, 0, (m_fontSize + 5.0f) * m_Text.length(), m_fontSize };
	else
		baseUI.m_OriginalBounds = { 0, 0, bounds.right, bounds.bottom };

	baseUI.Setup(baseUI.GetPosition(), baseUI.GetScale(), baseUI.GetRotation(), baseUI.GetVisibility(), baseUI.GetOpacity());
}

void UIText::Draw()
{
	if (baseUI.m_Visibility == true)
	{
		ID2D1RenderTarget* rt = ui.GetRenderTarget();
		rt->SetTransform(baseUI.m_Transform);
		D2D1_RECT_F tempBounds;

		SetBounds(tempBounds, baseUI.m_CurrentBounds);

		std::wstring textAsWString(m_Text.begin(), m_Text.end());
		
		rt->DrawTextW(textAsWString.c_str(), (UINT32)textAsWString.length(), m_TextFormat, tempBounds, ui.GetBrush(m_brush));
	}
}

void UIText::Release()
{
	if (m_TextFormat != nullptr)
	{
		m_TextFormat->Release();
		m_TextFormat = nullptr;
	}

	m_Text.~ML_String();
}

void UIImage::SetImage(const char* filepath, bool ignoreRename)
{
	if (filepath != "")
	{
		SetupImage(filepath, m_Bitmap);

		if (!ignoreRename)
			m_fileName = filepath;

		baseUI.m_OriginalBounds = { 0.0f, 0.0f, m_Bitmap->GetSize().width, m_Bitmap->GetSize().height };
	}
}

void UIImage::SetHoverImage(const char* filepath, bool ignoreRename)
{
	if (filepath != "")
	{
		if (!ignoreRename)
			m_hoverFileName = filepath;
	}
}

void UIImage::Draw()
{
	if (baseUI.m_Visibility == true)
	{
		ID2D1RenderTarget* rt = ui.GetRenderTarget();
		rt->SetTransform(baseUI.m_Transform);
		D2D1_RECT_F tempBounds;

		SetBounds(tempBounds, baseUI.m_CurrentBounds);
		rt->DrawBitmap(m_Bitmap, tempBounds, baseUI.m_Opacity, D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR, tempBounds);
	}
}

void UIImage::Release()
{
	if (m_Bitmap != nullptr)
	{
		m_Bitmap->Release();
		m_Bitmap = nullptr;
	}

	m_fileName.~ML_String();
	m_hoverFileName.~ML_String();
}

void UIComponent::Setup(const char* baseImageFilepath, const char* baseImageHoverFilepath, const char* text, DSFLOAT2 position, DSFLOAT2 scale,
	float fontSize, DWRITE_TEXT_ALIGNMENT textAlignment, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment, float rotation, bool visibility, float opacity)
{
	m_Images.Initialize();
	m_Texts.Initialize();

	m_BaseImage.SetImage(baseImageFilepath);
	m_BaseImage.SetHoverImage(baseImageHoverFilepath);

	bool vis = visibility;

	if (baseImageFilepath == "")
		vis = false;

	m_BaseImage.baseUI.Setup(position, scale, rotation, vis, opacity);

	m_BaseText.SetText(text, m_BaseImage.baseUI.GetOriginalBounds(), fontSize, textAlignment, paragraphAlignment);
	m_BaseText.baseUI.Setup(position, scale, rotation, visibility, opacity);

}

void UIComponent::DrawAll()
{
	m_BaseImage.Draw();

	for (uint32_t i = 0; i < m_Images.size(); i++)
		m_Images[i].Draw();

	m_BaseText.Draw();

	for (uint32_t i = 0; i < m_Texts.size(); i++)
		m_Texts[i].Draw();
}

void UIComponent::SetAllVisability(bool value)
{
	m_BaseImage.baseUI.SetVisibility(value);
	m_BaseText.baseUI.SetVisibility(value);

	for (uint32_t i = 0; i < m_Images.size(); i++)
		m_Images[i].baseUI.SetVisibility(value);

	for (uint32_t i = 0; i < m_Texts.size(); i++)
		m_Texts[i].baseUI.SetVisibility(value);

}

void UIComponent::AddImage(const char* imageFilepath, DSFLOAT2 position, DSFLOAT2 scale, bool translateText)
{
	m_Images.push_back();

	m_Images[m_Images.size() - 1].SetImage(imageFilepath);

	m_Images[m_Images.size() - 1].baseUI.Setup(position, scale, 
		m_Images[m_Images.size() - 1].baseUI.GetRotation(), 
		m_Images[m_Images.size() - 1].baseUI.GetVisibility(), 
		m_Images[m_Images.size() - 1].baseUI.GetOpacity());

	if (translateText && m_BaseText.baseUI.GetVisibility())
	{
		m_BaseText.SetText(m_BaseText.m_Text.c_str(), m_Images[m_Images.size() - 1].baseUI.GetOriginalBounds(), m_BaseText.m_fontSize, m_BaseText.m_textAlignment, m_BaseText.m_paragraphAlignment);
		m_BaseText.baseUI.SetScale(m_BaseText.baseUI.GetScale());
		m_BaseText.baseUI.SetPosition(m_BaseText.baseUI.GetPosition());
	}
}

void UIComponent::AddHoverImage(UIImage& image, const char* imageFilepath)
{
	image.m_hoverFileName = imageFilepath;
}

void UIComponent::AddText(const char* text, DSBOUNDS textBounds, DSFLOAT2 position, DSFLOAT2 scale, float fontSize, DWRITE_TEXT_ALIGNMENT textAlignment, DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment)
{
	m_Texts.push_back();

	m_Texts[m_Texts.size() - 1].baseUI.Setup(position, scale);
	
	m_Texts[m_Texts.size() - 1].SetText(text, textBounds, fontSize, textAlignment, paragraphAlignment);

	m_Texts[m_Texts.size() - 1].baseUI.SetScale(m_Texts[m_Texts.size() - 1].baseUI.GetScale());
	m_Texts[m_Texts.size() - 1].baseUI.SetPosition(m_Texts[m_Texts.size() - 1].baseUI.GetPosition());
}

void UIComponent::Release()
{
	m_BaseImage.Release();
	m_BaseText.Release();

	for (uint32_t i = 0; i < m_Images.size(); i++)
		m_Images[i].Release();

	for (uint32_t i = 0; i < m_Texts.size(); i++)
		m_Texts[i].Release();

	m_Texts.~ML_Vector();
	m_Images.~ML_Vector();
}

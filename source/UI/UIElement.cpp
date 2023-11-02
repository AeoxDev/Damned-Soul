#include "UIElement.h"

#include "SDLHandler.h"

#include <wincodec.h>
#include <d2d1.h>

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

void UpdateTransform(D2D1::Matrix3x2F& transform, DSFLOAT2 position, float rotation)
{
	transform = D2D1::Matrix3x2F::Scale(1.0f, 1.0f)
		* D2D1::Matrix3x2F::Translation(position.x, position.y)
		* D2D1::Matrix3x2F::Rotation(rotation, { sdl.WIDTH / 2.0f, sdl.HEIGHT / 2.0f });
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

	m_PixelCoords = { pixelCoords.x - (m_CurrentBounds.right / 2.0f) , pixelCoords.y - (m_CurrentBounds.bottom / 2.0f) };
	m_Position = position;

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

void UIText::SetText(const char* text, DSBOUNDS bounds)
{
	m_Text = text;

	if (bounds.right == 0)
		baseUI.m_OriginalBounds = { 0, 0, 20.0f * m_Text.length(), 25.0f };
	else
		baseUI.m_OriginalBounds = { 0, 0, bounds.right, bounds.bottom };
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
		rt->DrawTextW(textAsWString.c_str(), (UINT32)textAsWString.length(), ui.GetTextFormat(), tempBounds, ui.GetBrush());
	}
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

void UIComponent::Setup(const char* baseImageFilepath, const char* text, DSFLOAT2 position,
	DSFLOAT2 scale, float rotation, bool visibility, float opacity)
{

	if (baseImageFilepath != "")
	{
		m_BaseImage.SetImage(baseImageFilepath);
		m_BaseImage.baseUI.Setup(position, scale, rotation, visibility, opacity);
	}
	else
	{
		m_BaseImage.SetImage("TempBaseImage");
		m_BaseImage.baseUI.Setup(position, scale, rotation, visibility, opacity);
	}

	if (text != "")
	{
		m_Text.SetText(text, m_BaseImage.baseUI.GetOriginalBounds());
		m_Text.baseUI.Setup(position, scale, rotation, visibility, opacity);
	}
	else
		m_Text.baseUI.SetVisibility(false);
}

void UIComponent::AddImage(const char* imageFilepath, DSFLOAT2 position, DSFLOAT2 scale, bool translateText)
{
	m_Images.push_back();

	if (imageFilepath != "")
		m_Images[m_Images.size() - 1].SetImage(imageFilepath);
	else
		m_Images[m_Images.size() - 1].SetImage("TempBaseImage");

	m_Images[m_Images.size() - 1].baseUI.Setup(position, scale);

	if (translateText)
	{
		m_Text.SetText(m_Text.m_Text.c_str(), m_Images[m_Images.size() - 1].baseUI.GetOriginalBounds());
		m_Text.baseUI.SetScale(m_Text.baseUI.GetScale());
		m_Text.baseUI.SetPosition(m_Text.baseUI.GetPosition());
	}
}

void UIComponent::Release()
{
	if (m_BaseImage.m_Bitmap)
	{
		m_BaseImage.m_Bitmap->Release();
		m_BaseImage.m_Bitmap = nullptr;
		m_BaseImage.m_fileName.~ML_String();
	}

	for (UIImage image : m_Images)
	{
		if (image.m_Bitmap)
		{
			image.m_Bitmap->Release();
			image.m_Bitmap = nullptr;
			image.m_fileName.~ML_String();
		}
	}

	m_Text.m_Text.~ML_String();
	m_Images.~ML_Vector();
}

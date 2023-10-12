#include "UI/UIComponent.h"
#include "SDLHandler.h"

using namespace DirectX;

void UIComponent::UpdateTransform()
{
	m_Transform = D2D1::Matrix3x2F::Scale(m_Scale.x, m_Scale.y)
		* D2D1::Matrix3x2F::Translation(m_Position.x, m_Position.y)
		* D2D1::Matrix3x2F::Rotation(m_Rotation, { sdl.WIDTH / 2.0f, sdl.HEIGHT / 2.0f });
}

void UIComponent::SetTransform(XMFLOAT2 position, XMFLOAT2 scale, float rotation)
{

	SetScale(scale);
	SetPosition(position);
	SetRotation(rotation);
}

//UIComponent::UIComponent(XMFLOAT2 position, XMFLOAT2 scale, float rotation, bool visibility)
//	:m_Scale(scale), m_Rotation(rotation), m_Visibility(visibility)
//{
//	m_CurrentBounds = { 0, 0, 0, 0 };
//	m_OriginalBounds = { 0, 0, 0, 0 };
//	
//	m_Position = {0, 0};
//	
//	UpdateTransform();
//}

XMFLOAT2 UIComponent::GetPosition()
{
	return m_Position;
}

XMFLOAT2 UIComponent::GetScale()
{
	return m_Scale;
}

float UIComponent::GetRotation()
{
	return m_Rotation;
}

void UIComponent::Setup(DirectX::XMFLOAT2 scale, float rotation, bool visibility)
{
	m_Scale = scale;
	m_Rotation = rotation;
	m_Visibility = visibility;

	m_CurrentBounds = { 0, 0, 0, 0 };
	m_OriginalBounds = { 0, 0, 0, 0 };

	m_Position = { 0, 0 };

	UpdateTransform();
}

void UIComponent::SetPosition(XMFLOAT2 position)
{
	// calculate screen space to pixel coords
	// (-1,-1) -> (1,1) => (0,0) -> (width, height)
	// (-1,-1) is the bottom left coorner, (1, 1) is the top right coorner
	//int currentWindowWidth, currentWindowHeight;
	//SDL_GetWindowSize(sdl.sdlWindow, &currentWindowWidth, &currentWindowHeight);
	XMFLOAT2 pixelCoords = { (position.x + 1.0f) * 0.5f * sdl.BASE_WIDTH, (1.0f - position.y) * 0.5f * sdl.BASE_HEIGHT };

	m_Position = { pixelCoords.x - (m_CurrentBounds.right / 2.0f) * m_Scale.x , pixelCoords.y - (m_CurrentBounds.bottom / 2.0f) * m_Scale.y };
	
	UpdateTransform();
}

void UIComponent::SetScale(XMFLOAT2 scale)
{

	int currentWindowWidth, currentWindowHeight;
	SDL_GetWindowSize(sdl.sdlWindow, &currentWindowWidth, &currentWindowHeight);
	//XMFLOAT2 scaleCoords = { (float)currentWindowWidth / sdl.BASE_WIDTH, (float)currentWindowHeight / sdl.BASE_HEIGHT };
	XMFLOAT2 scaleCoords = { 1.0f, 1.0f };
	
	m_Scale.x = scale.x * scaleCoords.x;
	m_Scale.y = scale.y * scaleCoords.y;
	m_CurrentBounds.right = m_OriginalBounds.right * m_Scale.x;
	m_CurrentBounds.bottom = m_OriginalBounds.bottom * m_Scale.y;
	//std::cout << "Bounds: " << m_CurrentBounds.right << ", " << m_CurrentBounds.bottom << std::endl;
	UpdateTransform();
}

void UIComponent::SetScale(float scale)
{
	m_Scale = { scale, scale };
	m_CurrentBounds.right = m_OriginalBounds.right * m_Scale.x;
	m_CurrentBounds.bottom = m_OriginalBounds.bottom * m_Scale.y;
	UpdateTransform();
}

void UIComponent::SetRotation(float rotation)
{
	m_Rotation = rotation;
	UpdateTransform();
}

void UIComponent::SetVisibility(bool value)
{
	m_Visibility = value;
}

void UIComponent::ToggleVisibility()
{
	m_Visibility = (m_Visibility) ? false : true;
}

bool UIComponent::IsVisible()
{
	return m_Visibility;
}

bool UIComponent::Intersect(DirectX::XMINT2 mousePos)
{
	return (mousePos.x > m_Position.x) && (mousePos.x < m_Position.x + m_CurrentBounds.right) &&
		(mousePos.y > m_Position.y) && (mousePos.y < m_Position.y + m_CurrentBounds.bottom);
}

#include "Camera.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Helper.h"
#include "SDLHandler.h"
#include <DirectXMath.h>
#include "PointOfInterestComponent.h"


#define CAMERA_FOV 3.14f/16.f
#define CAMERA_PERSPECTIVE_DEPTH 512.0f
//The max zoom in
#define CAMERA_ZOOM_IN_LIMIT 3.14f/24.f
//THe max zoom out
#define CAMERA_ZOOM_OUT_LIMIT 3.14f/6.5f

struct CameraStruct
{
	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_lookAt;
	DirectX::XMFLOAT3 m_up;
	DirectX::XMFLOAT3 m_rotation;

	DirectX::XMFLOAT4X4 m_view;

	DirectX::XMFLOAT4X4 m_perspective;
	DirectX::XMFLOAT4X4 m_orthographic;

	int16_t m_cameraBufferIndex;

	float m_FOV;

	//If this is true, the camera will update the persepctive matrix on update call
	//if it is false, the camera will update the orthographic matrix on call.
	bool m_projectionType;
};

struct CameraConstantBuffer
{
	DirectX::XMFLOAT4 m_cameraPosition;
	DirectX::XMFLOAT4X4 m_viewMatrix;
	DirectX::XMFLOAT4X4 m_projectionMatrix;
};

PoolPointer<CameraStruct> GameCamera;
PoolPointer<CameraConstantBuffer> BufferData;

float orthWidth, orthHeight, orthDepth;

void Camera::SetPosition(const float x, const float y, const float z, const bool includeOffset)
{
	GameCamera->m_position.x = x + (includeOffset * CAMERA_OFFSET_X);
	GameCamera->m_position.y = y + (includeOffset * CAMERA_OFFSET_Y);
	GameCamera->m_position.z = z + (includeOffset * CAMERA_OFFSET_Z);

	BufferData->m_cameraPosition = DirectX::XMFLOAT4(GameCamera->m_position.x, GameCamera->m_position.y, GameCamera->m_position.z, 1.f);
}

void Camera::SetLookAt(const float x, const float y, const float z)
{
	GameCamera->m_lookAt.x = x;
	GameCamera->m_lookAt.y = y;
	GameCamera->m_lookAt.z = z;
}

void Camera::SetUp(const float x, const float y, const float z)
{
	GameCamera->m_up.x = x;
	GameCamera->m_up.y = y;
	GameCamera->m_up.z = z;
}

void Camera::SetRotation(const float x, const float y, const float z)
{
	GameCamera->m_rotation.x = x;
	GameCamera->m_rotation.y = y;
	GameCamera->m_rotation.z = z;
}

void Camera::SetFOV(const float radians)
{
	if (radians < CAMERA_ZOOM_IN_LIMIT)
	{
		GameCamera->m_FOV = CAMERA_ZOOM_IN_LIMIT;
	}
	else if (radians > CAMERA_ZOOM_OUT_LIMIT)
	{
		GameCamera->m_FOV = CAMERA_ZOOM_OUT_LIMIT;
	}
	else
	{
		GameCamera->m_FOV = radians;
	}
}

void Camera::SetWidth(const float& width)
{
	orthWidth = width;
}

void Camera::SetHeight(const float& height)
{
	orthHeight = height;
}
void Camera::SetOrthographicDepth(const float& d)
{
	orthDepth = d;
}

void Camera::AdjustPosition(const float x, const float y, const float z)
{
	GameCamera->m_position.x += x;
	GameCamera->m_position.y += y;
	GameCamera->m_position.z += z;

	BufferData->m_cameraPosition = DirectX::XMFLOAT4(GameCamera->m_position.x, GameCamera->m_position.y, GameCamera->m_position.z, 1.f);
}

void Camera::AdjustLookAt(const float x, const float y, const float z)
{
	GameCamera->m_lookAt.x += x;
	GameCamera->m_lookAt.y += y;
	GameCamera->m_lookAt.z += z;
}

void Camera::AdjustUp(const float x, const float y, const float z)
{
	GameCamera->m_up.x += x;
	GameCamera->m_up.y += y;
	GameCamera->m_up.z += z;
}

void Camera::AdjustRotation(const float x, const float y, const float z)
{
	GameCamera->m_rotation.x += x;
	GameCamera->m_rotation.y += y;
	GameCamera->m_rotation.z += z;
}

void Camera::AdjustFOV(const float radians)
{
	GameCamera->m_FOV += radians;
}

const DirectX::XMVECTOR Camera::GetPosition()
{
	return DirectX::XMVECTOR{ GameCamera->m_position.x, GameCamera->m_position.y, GameCamera->m_position.z, 1.0f };
}

const DirectX::XMVECTOR Camera::GetLookAt()
{
	return DirectX::XMVECTOR{ GameCamera->m_lookAt.x, GameCamera->m_lookAt.y, GameCamera->m_lookAt.z, 1.0f };
}

const DirectX::XMVECTOR Camera::GetUp()
{
	return DirectX::XMVECTOR{ GameCamera->m_up.x, GameCamera->m_up.y, GameCamera->m_up.z, 0.0f };
}

const DirectX::XMVECTOR Camera::GetRotation()
{
	return DirectX::XMVECTOR{ GameCamera->m_rotation.x, GameCamera->m_rotation.y, GameCamera->m_rotation.z, 0.0f };
}

const DirectX::XMMATRIX Camera::GetView()
{
	return DirectX::XMLoadFloat4x4(&GameCamera->m_view);
}

float Camera::GetFOV()
{
	return GameCamera->m_FOV;
}

const DirectX::XMMATRIX Camera::GetPerspective()
{
	return DirectX::XMLoadFloat4x4(&GameCamera->m_perspective);
}

const DirectX::XMMATRIX Camera::GetOrthographic()
{
	return DirectX::XMLoadFloat4x4(&GameCamera->m_orthographic);
}

int16_t Camera::GetCameraBufferIndex()
{
	return GameCamera->m_cameraBufferIndex;
}

void Camera::UpdateView()
{
	DirectX::XMMATRIX view;
	view = DirectX::XMMatrixLookAtLH(GetPosition(), GetLookAt(), GetUp());
	DirectX::XMStoreFloat4x4(&GameCamera->m_view, view);
	DirectX::XMStoreFloat4x4(&BufferData->m_viewMatrix, DirectX::XMMatrixTranspose(view));

	//Update constant buffer
	 UpdateConstantBuffer(GameCamera->m_cameraBufferIndex, &(*BufferData));
}

void Camera::UpdateProjection()
{
	if (GameCamera->m_projectionType)
	{
		DirectX::XMMATRIX proj;
		proj = DirectX::XMMatrixPerspectiveFovLH(GetFOV(), (float)sdl.WIDTH / (float)sdl.HEIGHT, 0.1f, CAMERA_PERSPECTIVE_DEPTH);//proj = DirectX::XMMatrixPerspectiveLH(1600.f, 900.f, 0.1f, 50.f);
		DirectX::XMStoreFloat4x4(&GameCamera->m_perspective, proj);
		DirectX::XMStoreFloat4x4(&BufferData->m_projectionMatrix, DirectX::XMMatrixTranspose(proj));
	}
	else
	{
		DirectX::XMMATRIX orth;
		orth = DirectX::XMMatrixOrthographicLH(orthWidth, orthHeight, 0.1f, orthDepth);
		DirectX::XMStoreFloat4x4(&GameCamera->m_orthographic, orth);
		DirectX::XMStoreFloat4x4(&BufferData->m_projectionMatrix, DirectX::XMMatrixTranspose(orth));
	}
	//NOTE TODO: CANT BIND MULTIPLE SHADERS
	//SHADER_TO_BIND_BUFFER flags = BIND_VERTEX | BIND_PIXEL;
	//Update Constant buffer
	UpdateConstantBuffer(GameCamera->m_cameraBufferIndex, &(*BufferData));
}

void Camera::ToggleProjection()
{
	//If projection is true, it will become false
	//If projection is false, it will become true
	GameCamera->m_projectionType = !GameCamera->m_projectionType;
}

void Camera::ResetCamera()
{
	GameCamera->m_projectionType = true;

	SetPosition(CAMERA_OFFSET_X, CAMERA_OFFSET_Y, CAMERA_OFFSET_Z, false);
	SetLookAt(0.f, 0.f, 0.f);
	SetUp(0.f, 1.f, 0.f);

	SetRotation(0.f, 0.f, 0.f);
	SetFOV(CAMERA_FOV);

	//Default done, update now


	//Update the view matrix before creating the buffer for the first time
	//UpdateView();

	//Prepare the buffer to creation
	//Update camera pos, view and projection
	BufferData->m_cameraPosition = DirectX::XMFLOAT4(GameCamera->m_position.x, GameCamera->m_position.y, GameCamera->m_position.z, 1.0f);
	
	UpdateView();
	UpdateProjection();
	UpdateConstantBuffer(GameCamera->m_cameraBufferIndex, &(BufferData->m_cameraPosition));
	SetConstantBuffer(GameCamera->m_cameraBufferIndex, BIND_VERTEX, 1);
}

void Camera::InitializeCamera()
{
	BufferData = MemLib::palloc(sizeof(CameraConstantBuffer));
	GameCamera = MemLib::palloc(sizeof(CameraStruct));

	GameCamera->m_cameraBufferIndex = CreateConstantBuffer(&(BufferData->m_cameraPosition), sizeof(CameraConstantBuffer));
	ResetCamera();

	//SHADER_TO_BIND_BUFFER flags = BIND_VERTEX | BIND_PIXEL;
	//GameCamera->m_cameraBufferIndex = CreateConstantBuffer(&(BufferData->m_cameraPosition), sizeof(CameraConstantBuffer), BIND_VERTEX, 0);
}

void Camera::FreeCamera()
{
	MemLib::pfree(GameCamera);
	MemLib::pfree(BufferData);
}

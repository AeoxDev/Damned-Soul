#include "Camera.h"
#include "MemLib/MemLib.hpp"
#include "D3D11Helper.h"
#include "SDLHandler.h"

PoolPointer<Camera> m_camera;
PoolPointer<CameraConstantBuffer> m_bufferData;

void SetPosition(float x, float y, float z)
{
	
	m_camera->position.x = x;
	m_camera->position.y = y;
	m_camera->position.z = z;
}

void SetLookAt(float x, float y, float z)
{
	m_camera->lookAt.x = x;
	m_camera->lookAt.y = y;
	m_camera->lookAt.z = z;
}

void SetUp(float x, float y, float z)
{
	m_camera->up.x = x;
	m_camera->up.y = y;
	m_camera->up.z = z;
}

void SetRotation(float x, float y, float z)
{
	m_camera->rotation.x = x;
	m_camera->rotation.y = y;
	m_camera->rotation.z = z;
}

void SetFOV(float radians)
{
	m_camera->FOV = radians;
}

void AdjustPosition(float x, float y, float z)
{
	m_camera->position.x += x;
	m_camera->position.y += y;
	m_camera->position.z += z;
}

void AdjustLookAt(float x, float y, float z)
{
	m_camera->lookAt.x += x;
	m_camera->lookAt.y += y;
	m_camera->lookAt.z += z;
}

void AdjustUp(float x, float y, float z)
{
	m_camera->up.x += x;
	m_camera->up.y += y;
	m_camera->up.z += z;
}

void AdjustRotation(float x, float y, float z)
{
	m_camera->rotation.x += x;
	m_camera->rotation.y += y;
	m_camera->rotation.z += z;
}

void AdjustFOV(float radians)
{
	m_camera->FOV += radians;
}

DirectX::XMVECTOR GetPosition()
{
	return DirectX::XMLoadFloat3(&m_camera->position);
}

DirectX::XMVECTOR GetLookAt()
{
	return DirectX::XMLoadFloat3(&m_camera->lookAt);
}

DirectX::XMVECTOR GetUp()
{
	return DirectX::XMLoadFloat3(&m_camera->up);
}

DirectX::XMVECTOR GetRotation()
{
	return DirectX::XMLoadFloat3(&m_camera->rotation);
}

DirectX::XMMATRIX GetView()
{
	return DirectX::XMLoadFloat4x4(&m_camera->view);
}

float GetFOV()
{
	return m_camera->FOV;
}

DirectX::XMMATRIX GetPerspective()
{
	return DirectX::XMLoadFloat4x4(&m_camera->perspective);
}

DirectX::XMMATRIX GetOrthographic()
{
	return DirectX::XMLoadFloat4x4(&m_camera->orthographic);
}

int16_t GetCameraBufferIndex()
{
	return m_camera->cameraBufferIndex;
}

void UpdateView()
{
	DirectX::XMMATRIX view;
	view = DirectX::XMMatrixLookAtLH(GetPosition(), GetLookAt(), GetUp());
	DirectX::XMStoreFloat4x4(&m_camera->view, view);
	DirectX::XMStoreFloat4x4(&m_bufferData->viewMatrix, DirectX::XMMatrixTranspose(view));

	//Update constant buffer
	m_camera->cameraBufferIndex = CreateConstantBuffer(&(m_bufferData->cameraPosition), sizeof(CameraConstantBuffer), BIND_VERTEX, 0);
}

void UpdateProjection()
{
	if (m_camera->projectionType)
	{
		DirectX::XMMATRIX proj;
		proj = DirectX::XMMatrixPerspectiveFovLH(GetFOV(), (float)sdl.WIDTH / (float)sdl.HEIGHT, 0.1f, 50.f);//proj = DirectX::XMMatrixPerspectiveLH(1600.f, 900.f, 0.1f, 50.f);
		DirectX::XMStoreFloat4x4(&m_camera->perspective, proj);
		DirectX::XMStoreFloat4x4(&m_bufferData->projectionMatrix, DirectX::XMMatrixTranspose(proj));
	}
	else
	{
		DirectX::XMMATRIX orth;
		orth = DirectX::XMMatrixOrthographicLH((float)sdl.WIDTH, (float)sdl.HEIGHT, 0.1f, 50.f);
		DirectX::XMStoreFloat4x4(&m_camera->orthographic, orth);
		DirectX::XMStoreFloat4x4(&m_bufferData->projectionMatrix, DirectX::XMMatrixTranspose(orth));
	}
	//SHADER_TO_BIND_BUFFER flags = BIND_VERTEX | BIND_PIXEL;
	//Update Constant buffer
	m_camera->cameraBufferIndex = CreateConstantBuffer(&(m_bufferData->cameraPosition), sizeof(CameraConstantBuffer), BIND_VERTEX, 0);
}

void ToggleProjection()
{
	//If projection is true, it will become false
	//If projection is false, it will become true
	m_camera->projectionType ? false : true;
}

void InitializeCamera()
{
	m_bufferData = MemLib::palloc(sizeof(CameraConstantBuffer));

	m_camera = MemLib::palloc(sizeof(Camera));
	m_camera->projectionType = true;
	
	SetPosition(0.f, 0.f, -10.f);
	SetLookAt(0.f, 0.f, 1.f);
	SetUp(0.f, 1.f, 0.f);

	SetRotation(0.f, 0.f, 0.f);
	SetFOV(3.14f/6.f);

	//Default done, update now


	//Update the view matrix before creating the buffer for the first time
	//UpdateView();

	//Prepare the buffer to creation
	//Update camera pos, view and projection
	m_bufferData->cameraPosition = DirectX::XMFLOAT4(m_camera->position.x, m_camera->position.y, m_camera->position.z, 1.0f);
	UpdateView();
	UpdateProjection();

	//SHADER_TO_BIND_BUFFER flags = BIND_VERTEX | BIND_PIXEL;
	m_camera->cameraBufferIndex = CreateConstantBuffer(&(m_bufferData->cameraPosition), sizeof(CameraConstantBuffer), BIND_VERTEX, 0);


}

void FreeCamera()
{
	MemLib::pfree(m_camera);
	MemLib::pfree(m_bufferData);
}

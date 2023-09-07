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

DirectX::XMMATRIX GetProjection()
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
	DirectX::XMVECTOR defaultForward, defaultUp, cameraTarget;
	DirectX::XMVECTOR up, pos;
	DirectX::XMMATRIX cameraRotationMatrix;// , vectorRotationMatrix;

	defaultForward = DirectX::XMVectorSet(0.f, 0.f, 1.0f, 0.f);
	defaultUp = DirectX::XMVectorSet(0.f, 1.f, 0.0f, 0.f);

	pos = DirectX::XMLoadFloat3(&m_camera->position);
	up = DirectX::XMLoadFloat3(&m_camera->up);

	//Create camera rotation matrix
	cameraRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_camera->rotation.x, m_camera->rotation.y, m_camera->rotation.z);

	//Calculate the cameras forward
	cameraTarget = DirectX::XMVector3TransformCoord(defaultForward, cameraRotationMatrix);
	//Add position on top of cameras forward
	cameraTarget = DirectX::XMVectorAdd(cameraTarget, pos);

	//Update cameras, up vector
	up = DirectX::XMVector3TransformCoord(up, cameraRotationMatrix);
	DirectX::XMStoreFloat3(&m_camera->up, up);

	DirectX::XMMATRIX view;
	view = DirectX::XMMatrixLookAtLH(GetPosition(), GetLookAt(), GetUp());
	DirectX::XMStoreFloat4x4(&m_camera->view, view);
	
	
	//Ask Quin, about this
	//vectorRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_camera->rotation.x, m_camera->rotation.y, 0.f);

	//XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f);
	//this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	//this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	//this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	//this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

void UpdateCamera()
{

}

void SwitchProjection()
{
	//If projection is true, it will become false
	//If projection is false, it will become true
	m_camera->projection ? false : true;
}

void InitializeCamera()
{
	m_bufferData = MemLib::palloc(sizeof(CameraConstantBuffer));

	m_camera = MemLib::palloc(sizeof(Camera));
	m_camera->projection = true;
	
	SetPosition(0.f, 0.f, -10.f);
	SetLookAt(0.f, 0.f, 1.f);
	SetUp(0.f, 1.f, 0.f);

	SetRotation(0.f, 0.f, 0.f);
	SetFOV(3.14f/6.f);

	DirectX::XMMATRIX proj;
	proj = DirectX::XMMatrixPerspectiveFovLH(GetFOV(),(float)sdl.WIDTH/(float)sdl.HEIGHT, 0.1f, 50.f);//proj = DirectX::XMMatrixPerspectiveLH(1600.f, 900.f, 0.1f, 50.f);
	DirectX::XMStoreFloat4x4(&m_camera->perspective, proj);

	DirectX::XMMATRIX orth;
	orth = DirectX::XMMatrixOrthographicLH((float)sdl.WIDTH, (float)sdl.HEIGHT, 0.1f, 50.f);
	DirectX::XMStoreFloat4x4(&m_camera->orthographic, orth);

	DirectX::XMMATRIX view;
	view = DirectX::XMMatrixLookAtLH(GetPosition(), GetLookAt(), GetUp());
	DirectX::XMStoreFloat4x4(&m_camera->view, view);

	//Update the view matrix before creating the buffer for the first time
	//UpdateView();

	//Prepare the buffer to creation
	m_bufferData->cameraPosition = DirectX::XMFLOAT4(m_camera->position.x, m_camera->position.y, m_camera->position.z, 1.0f);
	DirectX::XMStoreFloat4x4(&m_bufferData->viewMatrix, GetView());

	if (m_camera->projection)
		DirectX::XMStoreFloat4x4(&m_bufferData->projectionMatrix, GetProjection());
	else
		DirectX::XMStoreFloat4x4(&m_bufferData->projectionMatrix, GetOrthographic());

	DirectX::XMStoreFloat4x4(&m_bufferData->viewMatrix, DirectX::XMMatrixTranspose(view));
	DirectX::XMStoreFloat4x4(&m_bufferData->projectionMatrix, DirectX::XMMatrixTranspose(proj));

	//SHADER_TO_BIND_BUFFER flags = BIND_VERTEX | BIND_PIXEL;
	m_camera->cameraBufferIndex = CreateConstantBuffer(&(m_bufferData->cameraPosition), sizeof(CameraConstantBuffer), BIND_VERTEX, 0);


}

void FreeCamera()
{
	MemLib::pfree(m_camera);
	MemLib::pfree(m_bufferData);
}

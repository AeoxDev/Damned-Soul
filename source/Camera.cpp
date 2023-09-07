#include "Camera.h"
#include "MemLib/MemLib.hpp"

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
	return DirectX::XMLoadFloat4x4(&m_view);
}

DirectX::XMMATRIX GetPerspective()
{
	DirectX::XMMATRIX returnMatrix;

	if (m_camera->projection)
		returnMatrix = DirectX::XMLoadFloat4x4(&m_projection);
	else
		returnMatrix = DirectX::XMLoadFloat4x4(&m_orthographic);

	return returnMatrix;
}

DirectX::XMMATRIX GetProjection()
{
	return DirectX::XMLoadFloat4x4(&m_projection);
}

DirectX::XMMATRIX GetOrthographic()
{
	return DirectX::XMLoadFloat4x4(&m_orthographic);
}

void UpdateView()
{
	DirectX::XMVECTOR defaultForward, defaultUp, cameraTarget;
	DirectX::XMVECTOR up, pos;
	DirectX::XMMATRIX cameraRotationMatrix, vectorRotationMatrix;

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
	
	
	//Ask Quin, about this
	//vectorRotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(m_camera->rotation.x, m_camera->rotation.y, 0.f);

	//XMMATRIX vecRotationMatrix = XMMatrixRotationRollPitchYaw(this->rot.x, this->rot.y, 0.0f);
	//this->vec_forward = XMVector3TransformCoord(this->DEFAULT_FORWARD_VECTOR, vecRotationMatrix);
	//this->vec_backward = XMVector3TransformCoord(this->DEFAULT_BACKWARD_VECTOR, vecRotationMatrix);
	//this->vec_left = XMVector3TransformCoord(this->DEFAULT_LEFT_VECTOR, vecRotationMatrix);
	//this->vec_right = XMVector3TransformCoord(this->DEFAULT_RIGHT_VECTOR, vecRotationMatrix);
}

void SwitchProjection()
{
	//If projection is true, it will become false
	//If projection is false, it will become true
	m_camera->projection ? false : true;
}

void InitializeCamera()
{
	m_camera = MemLib::palloc(sizeof(Camera));
	m_camera->projection = true;
	
	SetPosition(0.f, 0.f, -10.f);
	SetLookAt(0.f, 0.f, 1.f);
	SetRotation(0.f, 1.f, 0.f);

	SetRotation(0.f, 0.f, 0.f);
	SetFOV(0.f);

	DirectX::XMMATRIX proj;
	proj = DirectX::XMMatrixPerspectiveLH(1600.f, 900.f, 0.1f, 50.f);
	DirectX::XMStoreFloat4x4(&m_projection, proj);

	DirectX::XMMATRIX orth;
	orth = DirectX::XMMatrixOrthographicLH(1600.f, 900.f, 0.1f, 50.f);
	DirectX::XMStoreFloat4x4(&m_orthographic, orth);

	DirectX::XMMATRIX view;
	view = DirectX::XMMatrixLookAtLH(GetPosition(), GetLookAt(), GetUp());
	DirectX::XMStoreFloat4x4(&m_view, view);
}

void FreeCamera()
{
	MemLib::pfree(m_camera);
}

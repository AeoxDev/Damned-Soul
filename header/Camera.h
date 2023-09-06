#pragma once
#include <DirectXMath.h>
#include "MemLib/PoolPointer.hpp"

struct Camera
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 lookAt;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 rotation;

	float FOV;

	//If this is true, the camera will update the projection matrix on update call
	//if it is false, the camera will update the orthographic matrix on call.
	bool projection;
};

PoolPointer<Camera> m_camera;

DirectX::XMFLOAT4X4 m_view;

DirectX::XMFLOAT4X4 m_projection;
DirectX::XMFLOAT4X4 m_orthographic;


void SetPosition(float x, float y, float z);
void SetLookAt(float x, float y, float z);
void SetUp(float x, float y, float z);
void SetRotation(float x, float y, float z);
void SetFOV(float radians);

void AdjustPosition(float x, float y, float z);
void AdjustLookAt(float x, float y, float z);
void AdjustUp(float x, float y, float z);
void AdjustRotation(float x, float y, float z);

void AdjustFOV(float radians);

DirectX::XMVECTOR GetPosition();
DirectX::XMVECTOR GetLookAt();
DirectX::XMVECTOR GetUp();
DirectX::XMVECTOR GetRotation();

DirectX::XMMATRIX GetView();
DirectX::XMMATRIX GetProjection();
DirectX::XMMATRIX GetOrthographic();

void UpdateView();

/// <summary>
/// Switches the camera between projection and orthographic
/// </summary>
void SwitchProjection();

/// <summary>
/// Initializes the camera with default values
/// Position = (0, 0, -10)
/// Up = (0, 1, 0)
/// LookAt = (0, 0, 1)
/// </summary>
void InitializeCamera();
void FreeCamera();

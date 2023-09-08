#pragma once
#include <DirectXMath.h>
#include "MemLib/PoolPointer.hpp"


struct Camera
{
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 lookAt;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 rotation;

	DirectX::XMFLOAT4X4 view;

	DirectX::XMFLOAT4X4 perspective;
	DirectX::XMFLOAT4X4 orthographic;

	int16_t cameraBufferIndex;

	float FOV;

	//If this is true, the camera will update the persepctive matrix on update call
	//if it is false, the camera will update the orthographic matrix on call.
	bool projectionType;
};

struct CameraConstantBuffer
{
	DirectX::XMFLOAT4 cameraPosition;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

extern PoolPointer<Camera> m_camera;


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
float GetFOV();

DirectX::XMMATRIX GetView();
DirectX::XMMATRIX GetPerspective();
DirectX::XMMATRIX GetOrthographic();

int16_t GetCameraBufferIndex();

void UpdateView();
/// <summary>
/// Updates either projection or orthographic, see function TogglePerspective()
/// </summary>
void UpdateProjection();

/// <summary>
/// Switches the camera between projection and orthographic
/// </summary>
void ToggleProjection();

/// <summary>
/// Initializes the camera with default values
/// Position = (0, 0, -10)
/// Up = (0, 1, 0)
/// LookAt = (0, 0, 1)
/// </summary>
void InitializeCamera();
void FreeCamera();

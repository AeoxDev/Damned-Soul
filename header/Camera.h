#pragma once
#include <DirectXMath.h>
//extern PoolPointer<CameraStruct> m_camera;

namespace Camera
{
	void SetPosition(const float x, const float y, const float z);
	void SetLookAt(const float x, const float y, const float z);
	void SetUp(const float x, const float y, const float z);
	void SetRotation(const float x, const float y, const float z);
	void SetFOV(const float radians);

	void AdjustPosition(const float x, const float y, const float z);
	void AdjustLookAt(const float x, const float y, const float z);
	void AdjustUp(const float x, const float y, const float z);
	void AdjustRotation(const float x, const float y, const float z);

	void AdjustFOV(const float radians);

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
}
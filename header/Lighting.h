#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <iostream>

struct LightingStruct
{
	DirectX::XMFLOAT4	ambientColor;
	DirectX::XMFLOAT4	diffuseColor;
	DirectX::XMFLOAT4	specularColor;
	float intensity;
};


bool CreateLightingConstantBuffer(ID3D11Device* device, ID3D11Buffer** constantBuffer);

void UpdateLightingBuffer(ID3D11DeviceContext* context, ID3D11Buffer* lightingConstantBuffer, LightingStruct& lightingData);
#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <iostream>

struct LightingStruct
{
	DirectX::XMFLOAT4	ambientColor; // Kind of like background lighting
	DirectX::XMFLOAT4	diffuseColor; // Color of the light
	DirectX::XMFLOAT4	specularColor; // reflection, shiny 
	float intensity; // focus of the light. Basically tigher/focused vs broader/stofter
};


bool CreateLightingConstantBuffer(ID3D11Device* device, ID3D11Buffer** constantBuffer); //create a lighting constant buffer

void UpdateLightingBuffer(ID3D11DeviceContext* context, ID3D11Buffer* lightingConstantBuffer, LightingStruct& lightingData); // update the values of the constant buffer
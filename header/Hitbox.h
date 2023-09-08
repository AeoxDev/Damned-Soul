#pragma once
#include <DirectXMath.h>
#include <vector>
#include <d3d11.h>
#include <string>
#include <iostream>
#include <fstream>

//Hitbox variables
struct Hitbox
{
	DirectX::XMFLOAT3 middlePoint;
	int isCube;
	float widthOrRadius;
	float height;
	float depth;
};

struct WorldMatrix
{
	DirectX::XMFLOAT4X4 worldMatrix;

	WorldMatrix(const DirectX::XMMATRIX& wM)
	{
		DirectX::XMStoreFloat4x4(&worldMatrix, wM);
	}
};

//Shaders to render the hitboxes
ID3D11VertexShader* vShader = nullptr;
ID3D11GeometryShader* gShader = nullptr;
ID3D11PixelShader* pShader = nullptr;

//List of all hitboxes
std::vector<Hitbox> hitboxes;

//Buffer, Input layout, and SRV used for Vertex Pulling
ID3D11Buffer* hitboxStructuredBuffer = nullptr;
ID3D11Buffer* worldMatrixBuffer = nullptr;
ID3D11InputLayout* hitboxInputLayout = nullptr;
ID3D11ShaderResourceView* hitboxStructuredSRV = nullptr;

//RasterState for wireframe rendering
ID3D11RasterizerState* hitboxWireframeRaster = nullptr;

//Delete this buffer later
ID3D11Buffer* viewAndProjMatrix = nullptr;


void CreateHitbox(ID3D11Device*& device, int isCube, std::vector<DirectX::XMFLOAT3> vertexBuffer);

void InitializeBufferAndSRV(ID3D11Device*& device);

void CreateShadersLayoutAndRasterState(ID3D11Device*& device);

void DebugRenderHitboxes(ID3D11DeviceContext*& immediateContext, std::vector<Hitbox> hitboxes);

void DestroyShaders();

void DestroyHitboxResources();

void DestroyBufferAndSRV();

void DestroyHitboxes();
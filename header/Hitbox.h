#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>

//Hitbox variables
struct Hitbox
{
	DirectX::XMFLOAT3 middlePoint;
	int isCube;
	float widthOrRadius;
	float height;
	float depth;
};

struct HitboxVisualizeVariables
{
	//Shaders to render the hitboxes
	ID3D11VertexShader* vShader = nullptr;
	ID3D11GeometryShader* gShader = nullptr;
	ID3D11PixelShader* pShader = nullptr;

	//Buffer, Input layout, and SRV used for Vertex Pulling
	std::vector<Hitbox> hitboxes;
	ID3D11Buffer* hitboxStructuredBuffer = nullptr;
	ID3D11InputLayout* hitboxInputLayout = nullptr;
	ID3D11ShaderResourceView* hitboxStructuredSRV = nullptr;

	//RasterState for wireframe rendering
	ID3D11RasterizerState* hitboxWireframeRaster = nullptr;
};

void CreateHitbox(int isCube, std::vector<DirectX::XMFLOAT3>& vertexBuffer);

bool SetupHitboxVisualizer();

void InitializeBufferAndSRV();

void UpdateHitboxBuffer();

void CreateShadersLayoutAndRasterState();

void DebugRenderHitbox(ID3D11Buffer*& worldMatrix, ID3D11Buffer*& viewAndProjectionMatrix);

void DestroyHitboxVisualizeVariables();
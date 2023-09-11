#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
//#include "EntityFramework.h"

//Real
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

/// <summary>
/// Create a circle hitbox on the given entity with a radius and offset.
/// The offset does not compensate for the rotation of the entity.
/// </summary>
/// <param name="radius">The size of the circle</param>
/// <param name="offsetX"></param>
/// <param name="offsetZ"></param>
/// <param name="entity"></param>
/// <returns>The id of the circle, use this to edit and destroy this hitbox for the given component</returns>
//int CreateHitbox(EntityID entity, float radius, float offsetX, float offsetZ);
/// <summary>
/// Create a convex shape with corners relative to the position of the entity, then offset.
/// </summary>
/// <param name="entity"></param>
/// <param name="offsetX"></param>
/// <param name="offsetZ"></param>
/// <param name="corners">Max size of CONVEX_CORNER_LIMIT, currently 8</param>
/// <param name="cornerPosX"></param>
/// <param name="cornerPosY"></param>
/// <returns></returns>
//int CreateHitbox(EntityID entity, int corners, float* cornerPosX, float* cornerPosY, float offsetX, float offsetZ);

void CreateHitbox(int isCube, std::vector<DirectX::XMFLOAT3>& vertexBuffer);

bool SetupHitboxVisualizer();

void InitializeBufferAndSRV();

void UpdateHitboxBuffer();

void CreateShadersLayoutAndRasterState();

void DebugRenderHitbox(ID3D11Buffer*& worldMatrix, ID3D11Buffer*& viewAndProjectionMatrix);

void DestroyHitboxVisualizeVariables();
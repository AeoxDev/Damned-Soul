#pragma once
#include <DirectXMath.h>
#include <d3d11.h>
#include <vector>
#include "EntityFramework.h"

//Real
//Hitbox variables
struct VisualHitbox
{
	DirectX::XMFLOAT4 color; //4
	DirectX::XMFLOAT3 middlePoint; //3
	float radius; //1
};

struct HitboxVisualizeVariables
{
	//Shaders to render the hitboxes
	ID3D11VertexShader* vShader = nullptr;
	ID3D11GeometryShader* gShader = nullptr;
	ID3D11PixelShader* pShader = nullptr;

	//Buffer, Input layout, and SRV used for Vertex Pulling
	std::vector<VisualHitbox> hitboxes;
	ID3D11Buffer* hitboxStructuredBuffer = nullptr;
	ID3D11InputLayout* hitboxInputLayout = nullptr;
	ID3D11ShaderResourceView* hitboxStructuredSRV = nullptr;

	//RasterState for wireframe rendering
	ID3D11RasterizerState* hitboxWireframeRaster = nullptr;
};

//CIRCLE COLLISION:

/// <summary>
/// Create a circle hitbox on the given entity with a radius and offset.
/// The offset does not compensate for the rotation of the entity.
/// </summary>
/// <param name="radius">The size of the circle</param>
/// <param name="offsetX"></param>
/// <param name="offsetZ"></param>
/// <param name="entity"></param>
/// <returns>The id of the circle, use this to edit and destroy this hitbox for the given component
/// Returns -1 if component does not exist
/// -2 if Maximum amount of hitboxes achieved</returns>
int CreateHitbox(Registry& registry,EntityID& entity, float radius, float offsetX, float offsetZ);
void RemoveHitbox(Registry& registry, EntityID& entity, int hitboxID);
//Manual Flag setting functions.

void SetHitboxActive(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsStage(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsWall(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsPlayer(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsEnemy(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsStaticHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsDynamicHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxIsMoveable(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);

void SetHitboxHitStage(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxHitWall(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxHitPlayer(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxHitEnemy(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxHitStaticHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);
void SetHitboxHitDynamicHazard(Registry& registry, EntityID& entity, int hitboxID, bool setFlag = true);

void UpdatePhysics(Registry& registry);

void SetCollisionEvent(Registry& registry, EntityID& entity, int hitboxID, void* function);

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
int CreateHitbox(Registry& registry, EntityID& entity, int corners, float cornerPosX[], float cornerPosZ[]);

void AddHitboxComponent(Registry& registry, EntityID& entity);

void CreateProximityHitbox(Registry& registry, EntityID& entity, std::string fileName = "default"); //File name without extension ending

void AddProximityHitboxComponent(Registry& registry, EntityID& entity);

bool SetupHitboxVisualizer(Registry& registry);

void InitializeBufferAndSRV(Registry& registry);

void UpdateHitboxBuffer(Registry& registry);

void CreateShadersLayoutAndRasterState();

void DebugRenderHitbox(ID3D11Buffer*& worldMatrix); //ID3D11Buffer*& viewAndProjectionMatrix);

void DestroyHitboxVisualizeVariables();

void SetupTestHitbox();


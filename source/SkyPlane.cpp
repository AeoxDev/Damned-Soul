#include "SkyPlane.h"
#include "Registry.h"
#include "Camera.h"
#include "Model.h"
#include "ModelComponent.h"
#include "Components\Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "SDLHandler.h"


EntityID planes[AMOUNT_OF_PLANES];

EntityID m_basePlane;
EntityID m_rocksPlane;
EntityID m_islandPlane;

RS_IDX m_skyPlaneRasterizer;
DSV_IDX m_skyPlaneDepth;
VS_IDX m_skyVS;
PS_IDX m_skyPS;
CB_IDX m_skyConst;
BS_IDX m_skyBlend;

void InitializeSky()
{
	m_basePlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);
	m_rocksPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);
	m_islandPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);

	registry.AddComponent<ModelBonelessComponent>(m_basePlane, LoadModel("3.mdl"));
	registry.AddComponent<ModelBonelessComponent>(m_rocksPlane, LoadModel("2.mdl"));
	registry.AddComponent<ModelBonelessComponent>(m_islandPlane, LoadModel("1.mdl"));

	TransformComponent baseTComp;
	TransformComponent rocksTComp;
	TransformComponent islandTComp;

	DirectX::XMFLOAT3 camPos;

	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	baseTComp.positionX = camPos.x; baseTComp.positionY = camPos.y; baseTComp.positionZ = camPos.z;
	baseTComp.offsetY = -247.f; baseTComp.offsetZ = 350.f; 
	baseTComp.scaleX = 4.0f; baseTComp.scaleY = 2.5f;
	baseTComp.facingY = 0.75f;

	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	rocksTComp.positionX = camPos.x; rocksTComp.positionY = camPos.y; rocksTComp.positionZ = camPos.z;
	rocksTComp.offsetY = -247.f; rocksTComp.offsetZ = 300.f;
	rocksTComp.scaleX = 4.0f; rocksTComp.scaleY = 2.5f;
	rocksTComp.facingY = 0.75f;

	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	islandTComp.positionX = camPos.x; islandTComp.positionY = camPos.y; islandTComp.positionZ = camPos.z;
	islandTComp.offsetY = -247.f; islandTComp.offsetZ = 250.f;
	islandTComp.scaleX = 4.0f; islandTComp.scaleY = 2.5f;
	islandTComp.facingY = 0.75f;

	registry.AddComponent<TransformComponent>(m_basePlane, baseTComp);
	registry.AddComponent<TransformComponent>(m_rocksPlane, rocksTComp);
	registry.AddComponent<TransformComponent>(m_islandPlane, islandTComp);

	registry.AddComponent<SkyPlaneComponent>(m_basePlane);
	registry.AddComponent<SkyPlaneComponent>(m_rocksPlane);
	registry.AddComponent<SkyPlaneComponent>(m_islandPlane);


	m_skyConst = CreateConstantBuffer(sizeof(int));
	m_skyPlaneRasterizer = CreateRasterizerState(false, true);
	m_skyPlaneDepth = CreateDepthStencil(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, true);
	m_skyBlend = CreateBlendState();

	m_skyVS = LoadVertexShader("SkyVS.cso", LAYOUT_DESC::DEFAULT);
	m_skyPS = LoadPixelShader("SkyPS.cso");

	planes[0] = m_basePlane;
	planes[1] = m_rocksPlane;
	planes[2] = m_islandPlane;
}

void ReleaseSky()
{
	registry.DestroyEntity(m_basePlane);
	registry.DestroyEntity(m_rocksPlane);
	registry.DestroyEntity(m_islandPlane);
}

void SwitchCamera()
{
}

void UpdateTransform(int layerNumber)
{
	TransformComponent* tComp = registry.GetComponent<TransformComponent>(m_islandPlane);
	DirectX::XMFLOAT3 camPos;

	switch (layerNumber)
	{
	case 0:
		tComp = registry.GetComponent<TransformComponent>(m_basePlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		tComp->positionX = camPos.x * 0.9f;
		tComp->positionY = camPos.y * 0.9f;
		tComp->positionZ = camPos.z * 0.9f;
		break;
	case 1:
		tComp = registry.GetComponent<TransformComponent>(m_rocksPlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		tComp->positionX = camPos.x * 0.8f;
		tComp->positionY = camPos.y * 0.8f;
		tComp->positionZ = camPos.z * 0.8f;
		break;
	case 2:
		tComp = registry.GetComponent<TransformComponent>(m_islandPlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		tComp->positionX = camPos.x * 0.7f;
		tComp->positionY = camPos.y * 0.7f;
		tComp->positionZ = camPos.z * 0.7f;
		break;
	default:
		break;
	}


}


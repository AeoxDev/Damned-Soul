#include "SkyPlane.h"
#include "Registry.h"
#include "Camera.h"
#include "Model.h"
#include "ModelComponent.h"
#include "Components\Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "SDLHandler.h"
#include "GameRenderer.h"


EntityID planes[AMOUNT_OF_PLANES];

EntityID m_basePlane;
EntityID m_rocksPlane;
EntityID m_islandPlane;

RS_IDX m_skyPlaneRasterizer;
DSS_IDX m_skyPlaneStencil;
VS_IDX m_skyVS;
PS_IDX m_skyPS;
PS_IDX m_skyPSForeground;
CB_IDX m_skyConst;
BS_IDX m_skyBlend;
SRV_IDX m_skyBackBufferSRVCopy;
SRV_IDX m_skyBackBufferSRV;


void InitializeSky()
{
	m_basePlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);
	m_rocksPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);
	m_islandPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);

	registry.AddComponent<ModelBonelessComponent>(m_basePlane, LoadModel("LavaIslandLayer.mdl"));
	registry.AddComponent<ModelBonelessComponent>(m_rocksPlane, LoadModel("IslandLayer.mdl"));
	registry.AddComponent<ModelBonelessComponent>(m_islandPlane, LoadModel("BackgroundLayer.mdl"));

	TransformComponent baseTComp;
	TransformComponent rocksTComp;
	TransformComponent islandTComp;

	DirectX::XMFLOAT3 camPos;

	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	/*baseTComp.positionX = camPos.x; baseTComp.positionY = camPos.y; baseTComp.positionZ = camPos.z;
	baseTComp.offsetY = -25.f; baseTComp.offsetZ = 450.f;
	baseTComp.scaleX = 7.f; baseTComp.scaleY = 3.0f;
	baseTComp.facingY = 0.75f;*/

	//rocksTComp.positionX = camPos.x; rocksTComp.positionY = camPos.y; rocksTComp.positionZ = camPos.z;
	/*rocksTComp.offsetY = -25.f; rocksTComp.offsetZ = 445.f;
	rocksTComp.scaleX = 7.f; rocksTComp.scaleY = 3.0f;
	rocksTComp.facingY = 0.75f;*/


	//islandTComp.positionX = camPos.x; islandTComp.positionY = camPos.y; islandTComp.positionZ = camPos.z;
	/*islandTComp.offsetY = -25.f; islandTComp.offsetZ = 440.f;
	islandTComp.scaleX = 7.f; islandTComp.scaleY = 3.0f;
	islandTComp.facingY = 0.75f;*/


	registry.AddComponent<TransformComponent>(m_basePlane, baseTComp);
	registry.AddComponent<TransformComponent>(m_rocksPlane, rocksTComp);
	registry.AddComponent<TransformComponent>(m_islandPlane, islandTComp);

	registry.AddComponent<SkyPlaneComponent>(m_basePlane);
	registry.AddComponent<SkyPlaneComponent>(m_rocksPlane);
	registry.AddComponent<SkyPlaneComponent>(m_islandPlane);


	m_skyConst = CreateConstantBuffer(sizeof(int));
	m_skyPlaneRasterizer = CreateRasterizerState(false, true);
	m_skyPlaneStencil = CreateDepthStencilState();
	m_skyBlend = CreateBlendState();

	m_skyVS = LoadVertexShader("SkyVS.cso", LAYOUT_DESC::DEFAULT);
	m_skyPS = LoadPixelShader("SkyPS.cso");
	m_skyPSForeground = LoadPixelShader("SkyPSForeground.cso");

	planes[0] = m_basePlane;
	planes[1] = m_rocksPlane;
	planes[2] = m_islandPlane;


	RESOURCE_FLAGS rFlags = (RESOURCE_FLAGS)(BIND_RENDER_TARGET | BIND_SHADER_RESOURCE | BIND_UNORDERED_ACCESS);
	m_skyBackBufferSRVCopy = CreateShaderResourceViewTexture(RESOURCES::RESOURCE_TEXTURE2D, rFlags, CPU_FLAGS::NONE, 1600.f, 900.f);
	m_skyBackBufferSRV = CreateShaderResourceViewTexture(renderStates[backBufferRenderSlot].renderTargetView, RESOURCE_FLAGS::BIND_RENDER_TARGET);
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
	DirectX::XMFLOAT3 camLook;		
	float scale = 1.f;

	switch (layerNumber)
	{
	case 0:
		tComp = registry.GetComponent<TransformComponent>(m_basePlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		tComp->positionX = camPos.x + (camPos.x - camLook.x);
		tComp->positionY = camPos.y + (camPos.y - camLook.y);
		tComp->positionZ = camPos.z + (camPos.z - camLook.z);


		//tComp->facingX = (camPos.x - camLook.x) * scale;

		//tComp->positionX = camPos.x* 0.9f - (camPos.x - camLook.x) * 2.f;
		//tComp->positionY = camPos.y* 0.9f - (camPos.y - camLook.y) * 2.f;
		//tComp->positionZ = camPos.z* 0.9f;

		//tComp->positionX = -800.f;
		//tComp->positionY = 200.f;
		//tComp->positionZ = camPos.z;

		break;
	case 1:
		tComp = registry.GetComponent<TransformComponent>(m_rocksPlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		tComp->positionX = camPos.x + (camPos.x - camLook.x);
		tComp->positionY = camPos.y + (camPos.y - camLook.y);
		tComp->positionZ = camPos.z + (camPos.z - camLook.z)*10.f;

		//tComp->facingX = (camPos.x - camLook.x) * scale;

		//tComp->positionX = camPos.x* 0.8f - (camPos.x - camLook.x) * 2.f;
		//tComp->positionY = camPos.y* 0.8f - (camPos.y - camLook.y) * 2.f;
		//tComp->positionZ = camPos.z* 0.8f;

		break;
	case 2:
		tComp = registry.GetComponent<TransformComponent>(m_islandPlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		tComp->positionX = camPos.x + (camPos.x - camLook.x)*10.f;
		tComp->positionY = camPos.y + (camPos.y - camLook.y)*10.f;
		tComp->positionZ = camPos.z + (camPos.z - camLook.z) * 10.f;

		//tComp->facingX = (camPos.x - camLook.x) * scale;

		//tComp->positionX = camPos.x * 0.7f - (camPos.x - camLook.x) * 2.f;
		//tComp->positionY = camPos.y * 0.7f - (camPos.y - camLook.y) * 2.f;
		//tComp->positionZ = camPos.z * 0.7f;
	 //
		//tComp = registry.GetComponent<TransformComponent>(m_islandPlane);
		//DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		//DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		//tComp->positionX = camPos.x + (camPos.x - camLook.x);
		//tComp->positionY = camPos.y + (camPos.y - camLook.y);
		//tComp->positionZ = camPos.z + (camPos.z - camLook.z);


		break;
	default:
		break;
	}


}


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
VS_IDX m_skyVSForeground;
PS_IDX m_skyPSForeground;
CB_IDX m_skyConst;
BS_IDX m_skyBlend;
SRV_IDX m_skyBackBufferSRVCopy;
SRV_IDX m_skyBackBufferSRV;


void InitializeSky()
{
	m_basePlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);
	//m_rocksPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);
	//m_islandPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);

	registry.AddComponent<ModelBonelessComponent>(m_basePlane, LoadModel("BackgroundLayer.mdl"));
	//registry.AddComponent<ModelBonelessComponent>(m_rocksPlane, LoadModel("IslandLayer.mdl"));
	//registry.AddComponent<ModelBonelessComponent>(m_islandPlane, LoadModel("LavaIslandLayer.mdl"));

	TransformComponent baseTComp;
	//TransformComponent rocksTComp;
	//TransformComponent islandTComp;

	DirectX::XMFLOAT3 camPos;

	//DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	///*baseTComp.positionX = camPos.x; baseTComp.positionY = camPos.y; baseTComp.positionZ = camPos.z;
	//baseTComp.offsetY = -25.f; baseTComp.offsetZ = 450.f;
	//baseTComp.scaleX = 7.f; baseTComp.scaleY = 3.0f;
	//baseTComp.facingY = 0.75f;*/
	//rocksTComp.positionX = camPos.x; rocksTComp.positionY = camPos.y; rocksTComp.positionZ = camPos.z;
	////rocksTComp.offsetY = -247.f; rocksTComp.offsetZ = 100.f;
	//rocksTComp.scaleX = 4.0f; rocksTComp.scaleY = 2.5f;
	////rocksTComp.facingY = 0.75f;


	//islandTComp.positionX = camPos.x; islandTComp.positionY = camPos.y; islandTComp.positionZ = camPos.z;
	//islandTComp.offsetY = CAMERA_OFFSET_Y * -0.05f; 
	//islandTComp.offsetZ = CAMERA_OFFSET_Z * -1.3f;
	//islandTComp.scaleX = scaleX; islandTComp.scaleY = scaleY;
	//islandTComp.facingY = 0.75f;


	registry.AddComponent<TransformComponent>(m_basePlane, baseTComp);
	//registry.AddComponent<TransformComponent>(m_rocksPlane, rocksTComp);
	//registry.AddComponent<TransformComponent>(m_islandPlane, islandTComp);

	registry.AddComponent<SkyPlaneComponent>(m_basePlane);
	//registry.AddComponent<SkyPlaneComponent>(m_rocksPlane);
	//registry.AddComponent<SkyPlaneComponent>(m_islandPlane);


	m_skyConst = CreateConstantBuffer(sizeof(int));
	m_skyPlaneRasterizer = CreateRasterizerState(false, true);
	m_skyPlaneStencil = CreateDepthStencilState();
	m_skyBlend = CreateBlendState();

	m_skyVS = LoadVertexShader("SkyVS.cso", LAYOUT_DESC::DEFAULT);
	m_skyPS = LoadPixelShader("SkyPS.cso");
	m_skyVSForeground = LoadVertexShader("SkyVSForeground.cso", LAYOUT_DESC::DEFAULT);
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
	DirectX::XMVECTOR camPosVec;
	DirectX::XMVECTOR camDirVec;
	DirectX::XMVECTOR camVec;
	DirectX::XMFLOAT3 camVal;
	float scale = 1.f;

	switch (layerNumber)
	{
	case 0:
		break;
	case 1:
		//tComp = registry.GetComponent<TransformComponent>(m_rocksPlane);
		//DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		//DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		//camPosVec = DirectX::XMLoadFloat3(&camPos);		
		//camDirVec = DirectX::XMLoadFloat3(&camLook);
		//camVec	= DirectX::XMVectorSubtract(camPosVec, camDirVec);
		//camVec = DirectX::XMVector3Normalize(camVec);


		//DirectX::XMStoreFloat3(&camVal, camVec);
		//
		//scale = 2.f;


		//tComp->positionX = camPos.x * 3.f;// +camVal.x * scale;
		//tComp->positionY = 0.0f;//camPos.y * 0.9f;// +camVal.y * scale;
		//tComp->positionZ = camPos.z * 0.2f;// +camVal.z * scale;

		tComp = registry.GetComponent<TransformComponent>(m_rocksPlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		//tComp->facingX = (camPos.x - camLook.x) * scale;

		tComp->positionX = camPos.x* 0.9f - (camPos.x - camLook.x) * 2.f;
		tComp->positionY = camPos.y* 0.9f - (camPos.y - camLook.y) * 2.f;
		tComp->positionZ = camPos.z* 0.9f;

		break;
	case 2:
		tComp = registry.GetComponent<TransformComponent>(m_rocksPlane);
		DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
		DirectX::XMStoreFloat3(&camLook, Camera::GetLookAt());

		camPosVec = DirectX::XMLoadFloat3(&camPos);
		camDirVec = DirectX::XMLoadFloat3(&camLook);
		camVec = DirectX::XMVectorSubtract(camPosVec, camDirVec);
		camVec = DirectX::XMVector3Normalize(camVec);

		camVal;
		DirectX::XMStoreFloat3(&camVal, camVec);

		scale = 1.f;

		tComp->positionX = camPos.x * 0.7f + camVal.x * scale;
		tComp->positionY = camPos.y * 0.7f + camVal.y * scale;
		tComp->positionZ = camPos.z * 0.7f + camVal.z * scale;
		break;
	default:
		break;
	}


}


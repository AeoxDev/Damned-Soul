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

	registry.AddComponent<ModelBonelessComponent>(m_basePlane, LoadModel("BackgroundLayer.mdl"));


	TransformComponent baseTComp;

	registry.AddComponent<TransformComponent>(m_basePlane, baseTComp);

	registry.AddComponent<SkyPlaneComponent>(m_basePlane);


	m_skyConst = CreateConstantBuffer(sizeof(int));
	m_skyPlaneRasterizer = CreateRasterizerState(false, true);
	m_skyPlaneStencil = CreateDepthStencilState();
	m_skyBlend = CreateBlendState();

	m_skyVS = LoadVertexShader("SkyVS.cso", LAYOUT_DESC::DEFAULT);
	m_skyPS = LoadPixelShader("SkyPS.cso");
}

void ReleaseSky()
{
	registry.DestroyEntity(m_basePlane);
}

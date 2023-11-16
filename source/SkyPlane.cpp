#include "SkyPlane.h"
#include "Registry.h"
#include "Camera.h"
#include "Model.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"
#include "D3D11Helper\D3D11Helper.h"
#include "SDLHandler.h"

EntityID  m_skyPlane;
RS_IDX m_skyPlaneRasterizer;
DSV_IDX m_skyPlaneDepth;
VS_IDX m_skyVS;
PS_IDX m_skyPS;
CB_IDX m_skyConst;

void InitializeSky()
{
	m_skyPlane = registry.CreateEntity(ENTITY_PERSISTENCY_TIER::ENT_PERSIST_GAME);

	registry.AddComponent<ModelBonelessComponent>(m_skyPlane, LoadModel("BackgroundQuad.mdl"));
	
	TransformComponent tComp;// = { 0 }; // Initializing to zero might be dumb;

	DirectX::XMFLOAT3 camPos;
	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	tComp.positionX = camPos.x;
	tComp.positionY = camPos.y;// - 100.0f;
	tComp.positionZ = camPos.z;// + 270.0f;
	tComp.offsetY = -247.f;
	tComp.offsetZ = 330.f;
	tComp.scaleX = 4.0f;//tComp.scaleX = 2.3f;//1.93f; 
	tComp.scaleY = 2.5f;//tComp.scaleY = 1.5f;//1.1f;
	tComp.facingY = 0.75f;

	m_skyConst = CreateConstantBuffer(sizeof(int));

	registry.AddComponent<TransformComponent>(m_skyPlane, tComp);

	m_skyPlaneRasterizer = CreateRasterizerState(false, true);
	m_skyPlaneDepth = CreateDepthStencil(sdl.BASE_WIDTH, sdl.BASE_HEIGHT, true);

	m_skyVS = LoadVertexShader("SkyVS.cso", LAYOUT_DESC::DEFAULT);
	m_skyPS = LoadPixelShader("SkyPS.cso");

	//RenderSetupComponent setupComponent;
	//setupComponent.vertexShaders = ;
	//setupComponent.pixelShaders;

	//RenderableComponent* rComp = { 0 };
	//rComp.
}

void ReleaseSky()
{
	registry.DestroyEntity(m_skyPlane);
}

void SwitchCamera()
{
}

void UpdateTransform()
{
	TransformComponent* tComp = registry.GetComponent<TransformComponent>(m_skyPlane);
	DirectX::XMFLOAT3 camPos;
	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	tComp->positionX = camPos.x * 0.9f;
	tComp->positionY = camPos.y * 0.9f;// - 100.0f;
	tComp->positionZ = camPos.z * 0.9f;// + 270.0f;

}


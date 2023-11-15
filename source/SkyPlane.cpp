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

void InitializeSky()
{
	m_skyPlane = registry.CreateEntity(ENT_PERSIST_LEVEL);

	registry.AddComponent<ModelBonelessComponent>(m_skyPlane, LoadModel("BackgroundQuad.mdl"));
	
	TransformComponent tComp = { 0 }; // Initializing to zero might be dumb;

	DirectX::XMFLOAT3 camPos;
	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	tComp.positionX = camPos.x;
	tComp.positionY = camPos.y;
	tComp.positionZ = camPos.z;
	tComp.scaleX = 150.0f; tComp.scaleY = 150.0f; tComp.scaleZ = 150.0f;
	//tComp.facingZ = -1.f;
	//tComp.facingY = 0.5; 

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


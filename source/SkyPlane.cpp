#include "SkyPlane.h"
//#include "EntityFramework.h"
#include "Registry.h"
#include "Camera.h"
#include "Model.h"
#include "ModelComponent.h"
#include "TransformComponent.h"
#include "RenderableComponent.h"

SkyPlane::SkyPlane()
{
	m_skyPlane = registry.CreateEntity();
	
	registry.AddComponent<ModelBonelessComponent>(m_skyPlane, LoadModel("SkyPlane.mdl"));
	TransformComponent tComp = { 0 }; // Initializing to zero might be dumb;

	DirectX::XMFLOAT3 camPos;
	DirectX::XMStoreFloat3(&camPos, Camera::GetPosition());
	tComp.positionX = camPos.x;
	tComp.positionY = camPos.y;
	tComp.positionZ = camPos.z;
	
	tComp.scaleX = 5.0f; tComp.scaleY = 5.0f; tComp.scaleZ = 5.0f;

	LoadTexture();
}

void SkyPlane::Release()
{
	registry.DestroyEntity(m_skyPlane);
}

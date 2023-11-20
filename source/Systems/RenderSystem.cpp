#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "GameRenderer.h"
#include "UIRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"
#include "RenderDepthPass.h"

// ARIAN SKREV DETTA OM DET ÄR DÅLIG KOD TA DET MED MIG 1V1 IRL
#include "SkyPlane.h"
#include "States/StateManager.h"

void Render()
{
	for (auto entity : View<TransformComponent, ModelBonelessComponent>(registry))
	{
		if (registry.GetComponent<SkyPlaneComponent>(entity) != nullptr)
			continue;

		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelBonelessComponent* mc = registry.GetComponent<ModelBonelessComponent>(entity);
		Light::SetGammaCorrection(mc->gammaCorrection);
		Light::SetColorHue(mc->colorMultiplicativeRed, mc->colorMultiplicativeGreen, mc->colorMultiplicativeBlue,
			mc->colorAdditiveRed, mc->colorAdditiveGreen, mc->colorAdditiveBlue);
		if (tc->offsetX != 0.0f)
		{
			tc->offsetY = 0.0f;
		}
		SetWorldMatrix(tc->positionX + tc->offsetX, tc->positionY + tc->offsetY, tc->positionZ + tc->offsetZ,
			tc->facingX, tc->facingY, -tc->facingZ,
			tc->scaleX * tc->offsetScaleX, tc->scaleY * tc->offsetScaleY, tc->scaleZ * tc->offsetScaleZ,
			SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
		SetVertexBuffer(LOADED_MODELS[mc->model].m_vertexBuffer);
		SetIndexBuffer(LOADED_MODELS[mc->model].m_indexBuffer);
		LOADED_MODELS[mc->model].RenderAllSubmeshes();
	}

	SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[1]);
	for (auto entity : View<TransformComponent, ModelSkeletonComponent, AnimationComponent>(registry))
	{
		
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelSkeletonComponent* mc = registry.GetComponent<ModelSkeletonComponent>(entity);
		AnimationComponent* ac = registry.GetComponent<AnimationComponent>(entity);
		Light::SetGammaCorrection(mc->gammaCorrection);
		Light::SetColorHue(mc->colorMultiplicativeRed, mc->colorMultiplicativeGreen, mc->colorMultiplicativeBlue,
			mc->colorAdditiveRed, mc->colorAdditiveGreen, mc->colorAdditiveBlue);
		if (tc->offsetX != 0.0f)
		{
			tc->offsetY = 0.0f;
		}
		SetWorldMatrix(tc->positionX + tc->offsetX, tc->positionY + tc->offsetY, tc->positionZ + tc->offsetZ,
			tc->facingX, tc->facingY, -tc->facingZ,
			tc->scaleX * tc->offsetScaleX, tc->scaleY * tc->offsetScaleY, tc->scaleZ * tc->offsetScaleZ,
			SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
		SetVertexBuffer(LOADED_MODELS[mc->model].m_vertexBuffer);
		SetIndexBuffer(LOADED_MODELS[mc->model].m_indexBuffer);

		// Render with data
		LOADED_MODELS[mc->model].RenderAllSubmeshes(ac->aAnim, ac->aAnimIdx, ac->aAnimTime);
	}


}
bool ShadowSystem::Update()
{
	//First set camera based on directional light
	DirectX::XMVECTOR previousPos = Camera::GetPosition();
	DirectX::XMVECTOR previousLookAt = Camera::GetLookAt();
	DirectX::XMVECTOR previousUp = Camera::GetUp();
	DirectX::XMFLOAT3 vData;
	
	float3 dir = GetLightDirection();
	DirectX::XMVECTOR cameraV = Camera::GetLookAt(); 
	DirectX::XMFLOAT3 cameraLookAt;
	DirectX::XMStoreFloat3(&cameraLookAt, cameraV);
	Camera::ToggleProjection();
	float dist = 80.0f;
	Camera::SetPosition(cameraLookAt.x + -dir.x * dist, cameraLookAt.y + -dir.y * dist, cameraLookAt.z + -dir.z * dist + 16.0f, false);//Set this to center of stage offset upwards
	Camera::SetLookAt(cameraLookAt.x, cameraLookAt.y, cameraLookAt.z + 16.0f);//Set to center of stage
	Camera::SetUp(0.0f, 1.0f, 0.0f);
	Camera::SetWidth(640.0f * Camera::GetFOV());//Set width (x) of orthogonal based on stage
	Camera::SetHeight(640.0f * Camera::GetFOV());//Set height (z) of orthogonal based on stage
	Camera::SetOrthographicDepth(256.0f);
	Camera::UpdateView();
	Camera::UpdateProjection();
	Camera::SaveToShadowMapCamera();
	int16_t cameraIdx = Camera::GetCameraBufferIndex();
	
	SetConstantBuffer(cameraIdx, SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 1);
	SetConstantBuffer(cameraIdx, SHADER_TO_BIND_RESOURCE::BIND_PIXEL, 1);

	//Shadow mapping: render geometry
	SetTopology(TRIANGLELIST);
	SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[0]);
	SetShadowmap(true);
	SetRasterizerState(renderStates[backBufferRenderSlot].rasterizerState);
	
	Render();

	//Return the camera
	Camera::ToggleProjection();
	DirectX::XMStoreFloat3(&vData, previousPos);
	Camera::SetPosition(vData.x, vData.y, vData.z, false);//Set this to center of stage offset upwards
	DirectX::XMStoreFloat3(&vData, previousLookAt);
	Camera::SetLookAt(vData.x, vData.y, vData.z);//Set to center of stage
	DirectX::XMStoreFloat3(&vData, previousUp);
	Camera::SetUp(vData.x, vData.y, vData.z);
	Camera::UpdateView();
	Camera::UpdateProjection();
	SetViewport(renderStates[backBufferRenderSlot].viewPort);
	return true;
}

// ARIAN SKREV DETTA OM DET ÄR DÅLIG KOD TA DET MED MIG 1V1 IRL
void RenderSkyPlane()
{
	int loops = 0;

	for (int i = 0; i < AMOUNT_OF_PLANES; i++)
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(planes[i]);
		ModelBonelessComponent* mc = registry.GetComponent<ModelBonelessComponent>(planes[i]);
		if (tc->offsetX != 0.0f)
		{
			tc->offsetY = 0.0f;
		}


		int* level = (int*)MemLib::spush(sizeof(int));
		*level = stateManager.activeLevel;

		UpdateConstantBuffer(m_skyConst, (void*)level);

		MemLib::spop();

		SetConstantBuffer(m_skyConst, BIND_VERTEX, 3);

		UpdateTransform(loops++);
		SetWorldMatrix(tc->positionX + tc->offsetX, tc->positionY + tc->offsetY, tc->positionZ + tc->offsetZ,
			tc->facingX, tc->facingY, -tc->facingZ,
			tc->scaleX * tc->offsetScaleX, tc->scaleY * tc->offsetScaleY, tc->scaleZ * tc->offsetScaleZ,
			SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
		SetVertexBuffer(LOADED_MODELS[mc->model].m_vertexBuffer);
		SetIndexBuffer(LOADED_MODELS[mc->model].m_indexBuffer);

		SetVertexShader(m_skyVS);
		SetPixelShader(m_skyPS);

		LOADED_MODELS[mc->model].RenderAllSubmeshes();

		// If this is the first loop, set the blend state as the last thing we do
		//+if (i == 0)
			//SetBlendState(m_skyBlend);

	}
	UnsetConstantBuffer(BIND_VERTEX, 3);
	//UnsetBlendState();
}

bool RenderSystem::Update()
{
	
	//Forward+ depth pass
	SetTopology(TRIANGLELIST);
	
	SetDepthPassTexture(true);
	SetRasterizerState(renderStates[backBufferRenderSlot].rasterizerState);
	SetPixelShader(GetDepthPassPixelShader());
	SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[0]);
	Render();
	ClearBackBuffer();
	// Render UI
	RenderUI();
	
	//Render Lightpass

	//Set shaders here.
	PrepareBackBuffer();
	//If light needs to update, update it.
	SetConstantBuffer(Light::GetLightBufferIndex(), BIND_PIXEL, 2);
	Light::UpdateLight();
	SetDepthPassTexture(false);
	SetShadowmap(false);


	// Set Geometry Shader used for normalmapping
	SetGeometryShader(renderStates[backBufferRenderSlot].geometryShader);
	// Render
	Render();
	// Unset geometry shader
	UnsetGeometryShader();

	// ARIAN SKREV DETTA OM DET ÄR DÅLIG KOD TA DET MED MIG 1V1 IRL
	RenderSkyPlane();
	

	//UpdateGlobalShaderBuffer();
	UnsetDepthPassTexture(false);
	UnsetShadowmap(false);
	return true;
}
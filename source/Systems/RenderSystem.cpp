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

enum RenderPass
{
	ShadowPass, DepthPass, LightPass
};

void Render(RenderPass renderPass)
{

	for (auto entity : View<TransformComponent, ModelBonelessComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelBonelessComponent* mc = registry.GetComponent<ModelBonelessComponent>(entity);
		if (renderPass  == ShadowPass && mc->castShadow == false)
		{
			continue;
		}
		
		// If this isn't a shadow pass, update colors (and reset temp colors)
		if (LightPass == renderPass)
		{
			Light::SetGammaCorrection(mc->shared.gammaCorrection);
			Light::SetColorHue(mc->shared.GetRedMult(), mc->shared.GetGreenMult(), mc->shared.GetBlueMult(),
				mc->shared.GetRedAdd(), mc->shared.GetGreenAdd(), mc->shared.GetBlueAdd());
			Light::UpdateLight();
			mc->shared.ResetTempColor();
		}

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

		// If this isn't a shadow pass, update colors (and reset temp colors)
		if (LightPass == renderPass)
		{
			Light::SetGammaCorrection(mc->shared.gammaCorrection);
			Light::SetColorHue(mc->shared.GetRedMult(), mc->shared.GetGreenMult(), mc->shared.GetBlueMult(),
				mc->shared.GetRedAdd(), mc->shared.GetGreenAdd(), mc->shared.GetBlueAdd());
			Light::UpdateLight();
			mc->shared.ResetTempColor();
		}

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
		if (entity.index)
		{
			LOADED_MODELS[mc->model].RenderAllSubmeshes(ac->aAnim, ac->aAnimIdx, ac->GetTimeValue());
		}
		else
		{
			LOADED_MODELS[mc->model].RenderAllSubmeshes(ac->aAnim, ac->aAnimIdx, ac->GetTimeValue());
		}
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
	
	Render(ShadowPass);

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
bool RenderSystem::Update()
{
	for (auto entity : View<TransformComponent, LightComponent>(registry))
	{
		TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
		//LightComponent* light = registry.GetComponent<LightComponent>(entity);
		//Use the offset from light.
		OffsetPosition(entity, transform->positionX, transform->positionY, transform->positionZ, transform->facingX, transform->facingY, transform->facingZ);
		OffsetFacing(entity, transform->facingX, transform->facingY, transform->facingZ);
	}
	//Forward+ depth pass
	SetTopology(TRIANGLELIST);
	
	SetDepthPassTexture(true);
	SetRasterizerState(renderStates[backBufferRenderSlot].rasterizerState);
	SetPixelShader(GetDepthPassPixelShader());
	SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[0]);
	Render(DepthPass);
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
	Render(LightPass);
	// Unset geometry shader
	UnsetGeometryShader();
	//UpdateGlobalShaderBuffer();
	UnsetDepthPassTexture(false);
	UnsetShadowmap(false);

	//Do the debugHitbox
#ifdef _DEBUG
	SetTopology(LINESTRIP);
	SetVertexShader(GetHitboxVisVertexShader());
	SetPixelShader(GetHitboxVisPixelShader());
	SetRasterizerState(GetHitboxRasterizerState());
	SetConstantBuffer(GetHitboxConstantBuffer(), BIND_VERTEX, 2);
	//Do the loop
	for (auto entity : View<TransformComponent, HitboxVisualComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		HitboxVisualComponent* hitboxV = registry.GetComponent<HitboxVisualComponent>(entity);
		for (size_t i = 0; i < SAME_TYPE_HITBOX_LIMIT; i++)
		{
			if (hitboxV->GetNrVertices(entity, i) > 0)
			{
				hitboxV->UpdateHitboxConstantBuffer(entity, i);
				SetWorldMatrix(tc->positionX + tc->offsetX, 0.6f, tc->positionZ + tc->offsetZ,
					tc->facingX, tc->facingY, -tc->facingZ,
					1.0f, 1.0f, 1.0f,
					SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);

				int vertices = hitboxV->GetNrVertices(entity, i);
				if (vertices > 0)
				{
					Render(vertices);
				}
			}
			
		}
		for (size_t i = SAME_TYPE_HITBOX_LIMIT; i < SAME_TYPE_HITBOX_LIMIT + SAME_TYPE_HITBOX_LIMIT; i++)
		{
			if (hitboxV->GetNrVertices(entity, i) > 0)
			{
				hitboxV->UpdateHitboxConstantBuffer(entity, i);
				SetWorldMatrix(tc->positionX + tc->offsetX, 0.6f, tc->positionZ + tc->offsetZ,
					tc->facingX, tc->facingY, -tc->facingZ,
					tc->scaleX, tc->scaleY, tc->scaleZ,
					SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);

				int vertices = hitboxV->GetNrVertices(entity, i);
				if (vertices > 0)
				{
					Render(vertices);
				}
			}
			
		}
		
		
	}
#endif // _DEBUG

	
	return true;
}
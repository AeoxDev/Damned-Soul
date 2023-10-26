#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "UIRenderer.h"
#include "Camera.h"
#include "Light.h"
#include "Model.h"

bool RenderSystem::Update()
{
	ClearBackBuffer();

	// Render UI
	RenderUI();

	//Render Geometry

	//Set shaders here.
	PrepareBackBuffer();
	//If light needs to update, update it.
	SetConstantBuffer(Light::GetLightBufferIndex(), BIND_PIXEL, 2);
	Light::UpdateLight();



	for (auto entity : View<TransformComponent, ModelBonelessComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelBonelessComponent* mc = registry.GetComponent<ModelBonelessComponent>(entity);

		Light::SetColorHue(mc->colorMultiplicativeRed, mc->colorMultiplicativeGreen, mc->colorMultiplicativeBlue,
			mc->colorAdditiveRed, mc->colorAdditiveGreen, mc->colorAdditiveBlue);
		if (tc->offsetX != 0.0f)
		{
			tc->offsetY = 0.0f;
		}
		SetWorldMatrix(tc->positionX + tc->offsetX, tc->positionY + tc->offsetY, tc->positionZ + tc->offsetZ, tc->facingX, tc->facingY, -tc->facingZ, tc->scaleX, tc->scaleY, tc->scaleZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX,0);
		SetVertexBuffer(LOADED_MODELS[mc->model].m_vertexBuffer);
		SetIndexBuffer(LOADED_MODELS[mc->model].m_indexBuffer);
		LOADED_MODELS[mc->model].RenderAllSubmeshes();
	}

	PrepareBackBuffer(true);
	for (auto entity : View<TransformComponent, ModelSkeletonComponent, AnimationComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelSkeletonComponent* mc = registry.GetComponent<ModelSkeletonComponent>(entity);
		AnimationComponent* ac = registry.GetComponent<AnimationComponent>(entity);

		Light::SetColorHue(mc->colorMultiplicativeRed, mc->colorMultiplicativeGreen, mc->colorMultiplicativeBlue,
			mc->colorAdditiveRed, mc->colorAdditiveGreen, mc->colorAdditiveBlue);
		if (tc->offsetX != 0.0f)
		{
			tc->offsetY = 0.0f;
		}
		SetWorldMatrix(tc->positionX + tc->offsetX, tc->positionY + tc->offsetY, tc->positionZ + tc->offsetZ, tc->facingX, tc->facingY, -tc->facingZ, tc->scaleX, tc->scaleY, tc->scaleZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
		SetVertexBuffer(LOADED_MODELS[mc->model].m_vertexBuffer);
		SetIndexBuffer(LOADED_MODELS[mc->model].m_indexBuffer);

		// Render with data
		LOADED_MODELS[mc->model].RenderAllSubmeshes(ac->aAnim, ac->aAnimIdx, ac->aAnimTime);
	}
	//UpdateGlobalShaderBuffer();
	return true;
}
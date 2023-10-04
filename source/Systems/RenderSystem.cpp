#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Components.h"
#include "D3D11Helper.h"
#include "GameRenderer.h"
#include "UIRenderer.h"

bool RenderSystem::Update()
{
	ClearBackBuffer();
	RenderUI();
	//Render Geometry

	//Set shaders here.
	PrepareBackBuffer();
	for (auto entity : View<TransformComponent, ModelBonelessComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelBonelessComponent* mc = registry.GetComponent<ModelBonelessComponent>(entity);

		SetWorldMatrix(tc->positionX, tc->positionY, tc->positionZ, tc->facingX, tc->facingY, -tc->facingZ, tc->scaleX, tc->scaleY, tc->scaleZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
		SetVertexBuffer(mc->model.m_vertexBuffer);
		SetIndexBuffer(mc->model.m_indexBuffer);
		mc->model.RenderAllSubmeshes();
	}

	PrepareBackBuffer(true);
	for (auto entity : View<TransformComponent, ModelSkeletonComponent>(registry))
	{
		TransformComponent* tc = registry.GetComponent<TransformComponent>(entity);
		ModelSkeletonComponent* mc = registry.GetComponent<ModelSkeletonComponent>(entity);

		SetWorldMatrix(tc->positionX, tc->positionY, tc->positionZ, tc->facingX, tc->facingY, -tc->facingZ, tc->scaleX, tc->scaleY, tc->scaleZ, SHADER_TO_BIND_RESOURCE::BIND_VERTEX);
		SetVertexBuffer(mc->model.m_vertexBuffer);
		SetIndexBuffer(mc->model.m_indexBuffer);
		mc->model.RenderAllSubmeshes();
	}
	return true;
}
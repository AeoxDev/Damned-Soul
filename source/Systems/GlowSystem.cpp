#include "Systems/Systems.h"
#include "Components/Components.h"
#include "Registry.h"
#include "EntityFramework.h"
#include "D3D11Helper/D3D11Helper.h"
#include "Model.h"
#include "GameRenderer.h"
#include "Glow.h"

bool GlowSystem::Update()
{
	Glow::ClearGlowRenderTarget();
	bool prepped = false;
	bool drawn = false;
	// For every entity with a glow component.
	for (auto entity : View<GlowComponent>(registry))
	{
		GlowComponent* glow_comp = registry.GetComponent<GlowComponent>(entity);

		// Prepare to draw meshes.
		SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[0]);
		ModelBonelessComponent* boneless_comp = registry.GetComponent<ModelBonelessComponent>(entity);
		TransformComponent* trans_comp = registry.GetComponent<TransformComponent>(entity);
		if (boneless_comp && trans_comp)
		{
			if (!prepped)
			{
				// Prepare for glow pass.
				Glow::PrepareGlowPass();
				Glow::UpdateGlowBuffer(glow_comp->m_r, glow_comp->m_g, glow_comp->m_b);
				prepped = true;
			}
			// Draw boneless models.
			if (trans_comp->offsetX != 0.0f)
			{
				trans_comp->offsetY = 0.0f;
			}
			SetWorldMatrix(trans_comp->positionX + trans_comp->offsetX, trans_comp->positionY + trans_comp->offsetY, trans_comp->positionZ + trans_comp->offsetZ,
				trans_comp->facingX, trans_comp->facingY, -trans_comp->facingZ,
				trans_comp->scaleX * trans_comp->offsetScaleX, trans_comp->scaleY * trans_comp->offsetScaleY, trans_comp->scaleZ * trans_comp->offsetScaleZ,
				SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
			SetVertexBuffer(LOADED_MODELS[boneless_comp->model].m_vertexBuffer);
			SetIndexBuffer(LOADED_MODELS[boneless_comp->model].m_indexBuffer);
			LOADED_MODELS[boneless_comp->model].RenderAllSubmeshes();
			drawn = true;
		}


		ModelSkeletonComponent* skel_comp = registry.GetComponent<ModelSkeletonComponent>(entity);
		AnimationComponent* anim_comp = registry.GetComponent<AnimationComponent>(entity);
		if (skel_comp && trans_comp && anim_comp)
		{
			if (!prepped)
			{
				// Prepare for glow pass.
				Glow::PrepareGlowPass();
				Glow::UpdateGlowBuffer(glow_comp->m_r, glow_comp->m_g, glow_comp->m_b);
				prepped = true;
			}
			// Draw models with skeleton.
			SetGeometryShader(renderStates[backBufferRenderSlot].geometryShader);
			SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[1]);
			if (trans_comp->offsetX != 0.0f)
			{
				trans_comp->offsetY = 0.0f;
			}
			SetWorldMatrix(trans_comp->positionX + trans_comp->offsetX, trans_comp->positionY + trans_comp->offsetY, trans_comp->positionZ + trans_comp->offsetZ,
				trans_comp->facingX, trans_comp->facingY, -trans_comp->facingZ,
				trans_comp->scaleX * trans_comp->offsetScaleX, trans_comp->scaleY * trans_comp->offsetScaleY, trans_comp->scaleZ * trans_comp->offsetScaleZ,
				SHADER_TO_BIND_RESOURCE::BIND_VERTEX, 0);
			SetVertexBuffer(LOADED_MODELS[skel_comp->model].m_vertexBuffer);
			SetIndexBuffer(LOADED_MODELS[skel_comp->model].m_indexBuffer);

			// Render with data
			LOADED_MODELS[skel_comp->model].RenderAllSubmeshes(anim_comp->aAnim, anim_comp->aAnimIdx, anim_comp->aAnimTime);

			UnsetGeometryShader();
			drawn = true;
		}
		
		
	}
	if (drawn)
	{
		// FInish glow and do blur.
		Glow::FinishGlowPass();
		Glow::PrepareBlurPass();
		Dispatch(50, 29, 1);
		Glow::FinishBlurPass();
	}

	return true;
}
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
	// Two passes:
	// 
	// 1. For every glow component (& transform and model, for security reasons and possible later UI glow pass? How do I check for either or?)
	// Can I use Render(), or do I need to create a separate version?

	Glow::ClearGlowRenderTarget();
	bool prepped = false;
	bool drawn = false;
	for (auto entity : View<GlowComponent>(registry))
	{
		GlowComponent* glow_comp = registry.GetComponent<GlowComponent>(entity);

		SetVertexShader(renderStates[backBufferRenderSlot].vertexShaders[0]);
		ModelBonelessComponent* boneless_comp = registry.GetComponent<ModelBonelessComponent>(entity);
		TransformComponent* trans_comp = registry.GetComponent<TransformComponent>(entity);
		if (boneless_comp && trans_comp)
		{
			if (!prepped)
			{
				Glow::PrepareGlowPass();
				Glow::UpdateGlowBuffer(glow_comp->m_r, glow_comp->m_g, glow_comp->m_b);
				prepped = true;
			}
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
				Glow::PrepareGlowPass();
				Glow::UpdateGlowBuffer(glow_comp->m_r, glow_comp->m_g, glow_comp->m_b);
				prepped = true;
			}
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
		Glow::FinishGlowPass();
		Glow::PrepareBlurPass();
		//Glow::UpdateBlurBuffer(1);
		Dispatch(50, 29, 1);
		Glow::FinishBlurPass();
	}



	// Should not need to repeat.
	//for (int i = 0; i < /*50*/1; i++)
	//{
	//	Glow::UpdateBlurBuffer(i);
	//	Dispatch(50, 29, 1);
	//	Glow::SwitchUAV();
	//	Glow::SetViews();
	//}



	// NOTE:
	// Read from glow/emission texture/map
	// (Get color from glow component)
	// Write to "sharp" bloom (do I need to handle overlaps? probably not)
	// 
	// 2. Once?
	// Read from "sharp"
	// Calculate falloff
	// Write to "soft" bloom
	// Repeat dispatch as much as necessary (5-9?)
	// 
	// Figure out where I apply glow to final render, probably not here? After scene, before UI. Separate function for UI glow?

	// THOUGHTS:
	// Sample from shadow map and make glow sources in shadow weaker? Does it matter? Would that look right?
	// Use depth to determine falloff factor?
	// 

	return true;
}
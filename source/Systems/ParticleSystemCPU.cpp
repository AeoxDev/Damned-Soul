#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Particles.h"
#include "Model.h"



bool ParticleSystemCPU::Update()
{
	//ResetGraphicsPipeline();

	ClearParticles();


	//Render
	for (auto pEntity : View<ParticleComponent, TransformComponent>(registry))
	{
		TransformComponent* tComp = registry.GetComponent<TransformComponent>(pEntity);
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);

		if (pComp->metadataSlot >= 0 && pComp->VFX == false)
		{
			SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			// Update the constant buffer to include start
			Particles::UpdateStart(pComp->metadataSlot);

			//Set all the shaders for regular particles
			Particles::PrepareParticlePass(renderStates, pComp->metadataSlot);

			RenderOffset((Particles::GetMetadataAtIndex(pComp->metadataSlot).end - Particles::GetMetadataAtIndex(pComp->metadataSlot).start), 0);

			Particles::FinishParticlePass();
		}
		else if (pComp->metadataSlot >= 0 && pComp->VFX == true)
		{
			// Update the constant buffer to include start
			Particles::UpdateStart(pComp->metadataSlot);

			//Set all the shaders for regular particles
			Particles::PrepareVFXPass(renderStates);

			RenderOffset((Particles::GetMetadataAtIndex(pComp->metadataSlot).end - Particles::GetMetadataAtIndex(pComp->metadataSlot).start), 0);

			Particles::FinishVFXPass();
		}


		if (pComp->modelUse)
		{
			SetWorldMatrix(tComp->positionX + pComp->meshOffset[0], tComp->positionY + pComp->meshOffset[1], tComp->positionZ + pComp->meshOffset[2], -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			//Set all the shaders for the VFX
			//Set a regular vertex shader then set all the other stuff
			Particles::PrepareMeshPass();
			

			// Check if offset can be teaked
			//SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			SetVertexBuffer(LOADED_MODELS[pComp->model].m_vertexBuffer);
			SetIndexBuffer(LOADED_MODELS[pComp->model].m_indexBuffer);

			//int indices = (int)LOADED_MODELS[pComp->model].m_data->GetIndices();
			//RenderIndexed(indices);

			LOADED_MODELS[pComp->model].RenderAllSubmeshes();

			Particles::FinishMeshPass();
		}
	}


	return true;
}
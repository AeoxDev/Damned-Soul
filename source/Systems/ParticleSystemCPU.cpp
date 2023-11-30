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

		if (pComp->metadataSlot >= 0)
		{
			SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			// Update the constant buffer to include start
			Particles::UpdateStart(pComp->metadataSlot);

			//Set all the shaders for regular particles
			Particles::PrepareParticlePass(pComp->metadataSlot);

			RenderOffset((Particles::GetMetadataAtIndex(pComp->metadataSlot).end - Particles::GetMetadataAtIndex(pComp->metadataSlot).start), 0);

			Particles::FinishParticlePass();
		}
		//else if (pComp->metadataSlot >= 0 && pComp->VFX == true)
		//{
		//	//SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

		//	// Update the constant buffer to include start
		//	Particles::UpdateStart(pComp->metadataSlot);

		//	//Set all the shaders for regular particles
		//	Particles::PrepareVFXPass(renderStates);

		//	RenderOffset((Particles::GetMetadataAtIndex(pComp->metadataSlot).end - Particles::GetMetadataAtIndex(pComp->metadataSlot).start), 0);

		//	Particles::FinishVFXPass();
		//}

		if (pComp->modelUse)
		{
			SetWorldMatrix(tComp->positionX + pComp->meshOffset[0], tComp->positionY + pComp->meshOffset[1], tComp->positionZ + pComp->meshOffset[2], -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			// Prepare the resources for the mesh pass
			Particles::PrepareMeshPass(pComp->metadataSlot);

			// The mesh pass does not use vertex pulling
			SetVertexBuffer(LOADED_MODELS[pComp->model].m_vertexBuffer);
			SetIndexBuffer(LOADED_MODELS[pComp->model].m_indexBuffer);

			/*Model doDis = LOADED_MODELS[pComp->model];
			doDis.m_indexBuffer*/
			LOADED_MODELS[pComp->model].RenderMeshes();


			Particles::FinishMeshPass();
		}
	}


	return true;
}
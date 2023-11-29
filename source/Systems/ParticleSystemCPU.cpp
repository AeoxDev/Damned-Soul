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


		SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

		if (pComp->metadataSlot >= 0)
		{
			//Set all the shaders for regular particles
			Particles::PrepareParticlePass(renderStates);

			// Update the constant buffer to include start
			Particles::UpdateStart(pComp->metadataSlot);


			RenderOffset((Particles::GetMetadataAtIndex(pComp->metadataSlot).end - Particles::GetMetadataAtIndex(pComp->metadataSlot).start),0);
		}

		SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

		if (pComp->modelUse)
		{
			//Set all the shaders for the VFX
			//Set a regular vertex shader then set all the other stuff
			Particles::PrepareVFXPass(renderStates);
			
			// Update the constant buffer to include mesh mid
			Particles::UpdateMid(pComp->meshMiddle);


			// Check if offset can be teaked
			//SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			SetVertexBuffer(LOADED_MODELS[pComp->model].m_vertexBuffer);
			SetIndexBuffer(LOADED_MODELS[pComp->model].m_indexBuffer);

			//int indices = (int)LOADED_MODELS[pComp->model].m_data->GetIndices();
			//RenderIndexed(indices);

			LOADED_MODELS[pComp->model].RenderAllSubmeshes();
		}
	}
	Particles::FinishParticlePass();

	return true;
}
#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Particles.h"
#include "Model.h"
#include "Camera.h"
#include "States/StateManager.h"


bool ParticleSystemCPU::Update()
{
	//ResetGraphicsPipeline();

	ClearParticles();


	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();


	//Render
	for (auto pEntity : View<ParticleComponent, TransformComponent>(registry))
	{
		TransformComponent* tComp = registry.GetComponent<TransformComponent>(pEntity);
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);

		float distX = (cameraPos.x - tComp->positionX);
		float distZ = (cameraPos.z - CAMERA_OFFSET_Z - tComp->positionZ);
		float dist = (distX * distX) + (distZ * distZ);
		if (dist > 6400.0f && stateManager.activeLevel != 5)
		{
			continue;
		}

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

			// TODO: ANGLE +Z & -X
			float angle = 0.0f;
			if ((tComp->facingZ <= 0.0f && tComp->facingX <= 0.0f) || (tComp->facingZ <= 0.0f && tComp->facingX >= 0.0f))
			{
				 angle = atanf(-1.0f * (tComp->facingX / tComp->facingZ));
			}
			else if (tComp->facingZ >= 0.0f && tComp->facingX <= 0.0f)
			{
				angle = std::clamp(atanf(tComp->facingZ / tComp->facingX), -1.0f, 1.0f);
			}
			else
			{
				angle = atanf(tComp->facingZ / tComp->facingX);
			}


			SetWorldMatrix(tComp->positionX + pComp->meshOffset[0], tComp->positionY + pComp->meshOffset[1], tComp->positionZ + pComp->meshOffset[2], angle, BIND_VERTEX, 0, true);
			//SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			// Prepare the resources for the mesh pass
			Particles::PrepareMeshPass(pComp->metadataSlot, *pComp);

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
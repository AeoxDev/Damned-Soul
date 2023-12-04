#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Particles.h"
#include "Model.h"
#include "Camera.h"
#include "States/StateManager.h"
#include "MemLib\ML_Vector.hpp"
#include "MemLib\ML_Map.hpp"
#include <algorithm>

bool ParticleSystemCPU::Update()
{

	ClearParticles();



	DirectX::XMFLOAT3 cameraPos = Camera::GetPositionFloat();

	
	//ML_Map<float, ML_Vector<EntityID>> tempArr = ML_Map<float, ML_Vector<EntityID>>();
	ML_Vector<EntityID> tempArr = ML_Vector<EntityID>();


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

		if (tempArr.size() == 0)
			tempArr.push_back(pEntity);
		else
		{
			int i = 0;
			bool added = false;
			for (; i < tempArr.size(); i++)
			{
				TransformComponent* entTransform = registry.GetComponent<TransformComponent>(tempArr[i]);


				if (tComp->positionZ > entTransform->positionZ)
				{
					tempArr.emplace(i, pEntity);
					added = true;
					break;
				}
			}
			if (!added)
				tempArr.push_back(pEntity);
		}



		//if (tempArr.contains(tComp->positionZ) == false)
		//{
		//	ML_Vector<EntityID> tempVec;
		//	tempVec.push_back(pEntity);
		//	tempArr.emplace(tComp->positionZ, tempVec);
		//}
		//else
		//{
		//	tempArr[tComp->positionZ].push_back(pEntity);
		//}
	}


	for (EntityID ents : tempArr)
	{
		Particles::CopyBackBufferToRender();

		TransformComponent* tComp = registry.GetComponent<TransformComponent>(ents);
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(ents);


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

		if (pComp->modelUse)
		{

			// Sets rotation based on the facing directions
			float angle = atan2f(tComp->facingZ, tComp->facingX);

			SetWorldMatrix(tComp->positionX + pComp->meshOffset[0], tComp->positionY + pComp->meshOffset[1], tComp->positionZ + pComp->meshOffset[2], angle, BIND_VERTEX, 0, true);
			//SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

			// Prepare the resources for the mesh pass
			Particles::PrepareMeshPass(pComp->metadataSlot, *pComp, timeCap);

			// The mesh pass does not use vertex pulling
			SetVertexBuffer(LOADED_MODELS[pComp->model].m_vertexBuffer);
			SetIndexBuffer(LOADED_MODELS[pComp->model].m_indexBuffer);

			/*Model doDis = LOADED_MODELS[pComp->model];
			doDis.m_indexBuffer*/
			LOADED_MODELS[pComp->model].RenderMeshes();


			Particles::FinishMeshPass();
		}

		Particles::CopyRenderToBackBuffer();
	}



	return true;
}
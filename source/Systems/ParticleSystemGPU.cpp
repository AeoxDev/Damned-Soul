#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Particles.h"


bool ParticleSystemGPU::Update()
{
	int highestActiveSlot = -1;
	int highestGroupAmount = -1;


	for (auto pEntity : View<ParticleComponent>(registry))
	{
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);
		if (pComp->metadataSlot >= 0)
		{
			if (highestActiveSlot < pComp->metadataSlot)
				highestActiveSlot = pComp->metadataSlot;

			if (highestGroupAmount < pComp->groupsRequiered)
				highestGroupAmount = pComp->groupsRequiered;

			//if (pComp->vertexReset == false)
			//	pComp->ResetVertex();

			//if (pComp->bufferReset == false)
			//	pComp->ResetBuffer();
		}
	}

	if (highestActiveSlot >= 0)
	{
		Particles::PrepareParticleCompute(renderStates);

		Dispatch((highestGroupAmount + 1), (highestActiveSlot + 1), 1); //x * y * z

		Particles::FinishParticleCompute(renderStates);
	}

	return true;
}

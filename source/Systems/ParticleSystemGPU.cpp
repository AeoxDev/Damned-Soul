#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper.h"
#include "Particles.h"


bool ParticleSystemGPU::Update()
{
	int activeMetadata = 0;
	for (auto pEntity : View<ParticleComponent>(registry))
	{
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);
		if (pComp->metadataSlot >= 0)
		{
			activeMetadata++;
		}
	}

	Particles::PrepareParticleCompute(renderStates);
	Dispatch(256, activeMetadata, 0);
	Particles::FinishParticleCompute(renderStates);

	return true;
}

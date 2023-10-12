#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper.h"
#include "Particles.h"



bool ParticleSystemCPU::Update()
{
	//ResetGraphicsPipeline();

	ClearParticles();

	//Set all the shaders
	Particles::PrepareParticlePass(renderStates);
	//Loop to find all metadata that are active,
	//Render
	for (auto pEntity : View<ParticleComponent>(registry))
	{
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);
		if (pComp->metadataSlot >= 0)
		{
			RenderOffset(THREADS_PER_GROUP, pComp->metadataSlot * THREADS_PER_GROUP);
		}
	}
	Particles::FinishParticlePass();

	return true;
}
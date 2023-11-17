#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "Components.h"
#include "D3D11Helper\D3D11Helper.h"
#include "Particles.h"



bool ParticleSystemCPU::Update()
{
	//ResetGraphicsPipeline();

	ClearParticles();

	//Set all the shaders
	Particles::PrepareParticlePass(renderStates);

	//Render
	for (auto pEntity : View<ParticleComponent, TransformComponent>(registry))
	{
		TransformComponent* tComp = registry.GetComponent<TransformComponent>(pEntity);
		ParticleComponent* pComp = registry.GetComponent<ParticleComponent>(pEntity);

		Particles::UpdateSingularMetadata(pComp->metadataSlot);

		SetWorldMatrix(tComp->positionX, tComp->positionY, tComp->positionZ, -tComp->facingX, tComp->facingY, tComp->facingZ, BIND_VERTEX, 0);

		if (pComp->metadataSlot >= 0)
		{
			RenderOffset((Particles::GetMetadataAtIndex(pComp->metadataSlot).end - Particles::GetMetadataAtIndex(pComp->metadataSlot).start),0);
		}
	}
	Particles::FinishParticlePass();

	return true;
}
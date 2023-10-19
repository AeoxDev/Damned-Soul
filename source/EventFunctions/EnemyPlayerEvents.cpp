#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "RelicFunctions.h"
#include "Relics/RelicFuncInputTypes.h" //Why isn't this included by RelicFunctions? Hermaaaaaaaaan

void BeginHit(EntityID& entity)
{
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	stats->UpdateHealth(-attackerStats->damage);

	// On Hit Relics
	auto funcVector = Relics::GetFunctionsOfType(Relics::FUNC_ON_WEAPON_HIT);
	RelicInput::OnHitInput funcInput
	{
		cpc->params.entity1,
		entity
	};
	for (uint32_t i = 0; i < funcVector.size(); ++i)
	{
		funcVector[i](&funcInput);
	}

	//Disable damage taken until EndHit
	SetHitboxCanTakeDamage(entity, 1, false); //Hardcode 1 since we know soft hitbox is always id 1. ahaaaa
}

void MiddleHit(EntityID& entity)
{

}

void EndHit(EntityID& entity)
{
	//Some hue shift make sure thing
	SetHitboxCanTakeDamage(entity, 1, true);
}

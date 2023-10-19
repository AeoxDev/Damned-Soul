#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "RelicFunctions.h"
#include "Relics/RelicFuncInputTypes.h" //Why isn't this included by RelicFunctions? Hermaaaaaaaaan
#include "DeltaTime.h"
#define KNOCKBACK_FACTOR 0.3f

void BeginHit(EntityID& entity)
{
	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	//Deal regular damage as well as on-hit damage from potential relics
	stats->UpdateHealth(-attackerStats->damage);
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
	SetHitboxCanTakeDamage(entity, 1, false); //We know soft hitbox is always id 1

	//Become red
	if (skelel)
		skelel->colorAdditiveRed = 1.0f;
	if (bonel)
		bonel->colorAdditiveRed = 1.0f;
}

void MiddleHit(EntityID& entity)
{
	//Reduce the hue shift gradually over time
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	if(skelel)
		skelel->colorAdditiveRed -= GetDeltaTime();
	if (bonel)
		bonel->colorAdditiveRed -= GetDeltaTime();

	//Take knockback
	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);
	if (cpc && transform)
	{
		float knockbackFactor = KNOCKBACK_FACTOR * sqrtf(attackerStats->damage) / (0.1f + transform->mass * GetEventTimedElapsed(entity));
		knockbackFactor *= knockbackFactor;
		transform->positionX += cpc->params.normal1X * GetDeltaTime() * knockbackFactor;
		transform->positionZ += cpc->params.normal1Z * GetDeltaTime() * knockbackFactor;
	}

	//GetElapsedTime to make the color flash instead
}

void EndHit(EntityID& entity)
{
	//Enable damage taken again
	SetHitboxCanTakeDamage(entity, 1, true);

	//Make sure we're back to our regular color
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	if (skelel)
		skelel->colorAdditiveRed = 0.0f;
	if (bonel)
		bonel->colorAdditiveRed = 0.0f;
}

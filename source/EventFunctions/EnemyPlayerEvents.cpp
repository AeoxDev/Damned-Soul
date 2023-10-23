#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "RelicFunctions.h"
#include "Relics/RelicFuncInputTypes.h" //Why isn't this included by RelicFunctions? Hermaaaaaaaaan
#include "DeltaTime.h"
#include <cmath> //sin

#define KNOCKBACK_FACTOR 0.3f

void BeginHit(EntityID& entity, const int& index)
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
}

void MiddleHit(EntityID& entity, const int& index)
{
	//Flash color red repeatedly
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	float frequency = 10.0f; //Higher frequency = faster flashing lights
	float cosineWave = std::cosf(GetEventTimedElapsed(entity, index) * frequency) * std::cosf(GetEventTimedElapsed(entity, index) * frequency);
	if (skelel)
		skelel->colorAdditiveRed = cosineWave;
	if (bonel)
		bonel->colorAdditiveRed = cosineWave;

	//Take knockback
	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);

	if ((!skelel && !bonel) || !cpc || !transform)
	{
		CancelTimedEvent(entity, index);
		return;
	}

	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);
	if (cpc && transform)
	{
		float knockbackFactor = KNOCKBACK_FACTOR / (0.1f + transform->mass * GetEventTimedElapsed(entity, index));
		knockbackFactor *= knockbackFactor;
		transform->positionX += cpc->params.normal1X * GetDeltaTime() * knockbackFactor;
		transform->positionZ += cpc->params.normal1Z * GetDeltaTime() * knockbackFactor;
	}
}

void EndHit(EntityID& entity, const int& index)
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


void HazardBeginHit(EntityID& entity, const int& index)
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

	//Become red
	if (skelel)
		skelel->colorAdditiveRed = 1.0f;
	if (bonel)
		bonel->colorAdditiveRed = 1.0f;
}
void HazardEndHit(EntityID& entity, const int& index)
{

	//Make sure we're back to our regular color
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	if (skelel)
		skelel->colorAdditiveRed = 0.0f;
	if (bonel)
		bonel->colorAdditiveRed = 0.0f;
}
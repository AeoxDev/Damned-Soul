#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "Relics/RelicFunctions.h"
#include "Relics/RelicFuncInputTypes.h" //Why isn't this included by RelicFunctions? Hermaaaaaaaaan
#include "DeltaTime.h"
#include "Levels/LevelHelper.h"
#include "UIRenderer.h"
//#include <cmath> //sin

#define KNOCKBACK_FACTOR 0.3f

void HitInteraction(const EntityID& attacker, StatComponent* attackerStats, const EntityID& defender, StatComponent* defenderStats)
{
	PlayerComponent* player = registry.GetComponent<PlayerComponent>(defender);
	//Deal regular damage as well as on-hit damage from potential relics

	// Calculate damage
	// Some values can start with default
	RelicInput::OnDamageCalculation funcInput;
	funcInput.attacker = attacker;
	funcInput.defender = defender;
	funcInput.damage = attackerStats->GetDamage();
	funcInput.cap = defenderStats->GetHealth();

	// Apply on damage calc functions
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_CALC))
		func(&funcInput);

	// Apply on damage final
	for (auto func : Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY))
		func(&funcInput);

	// Update health
	defenderStats->UpdateHealth(-1.f * funcInput.CollapseDamage(), player != nullptr);

	RedrawUI();
}


void BeginHit(EntityID& entity, const int& index)
{
	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	//ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	//ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	HitInteraction(cpc->params.entity1, attackerStats, entity, stats);

	//PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
	////Deal regular damage as well as on-hit damage from potential relics

	//// Calculate damage
	//float calculatedDamage = attackerStats->CalculateDamageDealt(stats);

	//stats->UpdateHealth(-calculatedDamage, player != nullptr);
	//auto funcVector = Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY);
	//RedrawUI();
	//RelicInput::OnHitInput funcInput
	//{
	//	cpc->params.entity1,
	//	entity
	//};
	//for (uint32_t i = 0; i < funcVector.size(); ++i)
	//{
	//	funcVector[i](&funcInput);
	//}

	//Disable damage taken until EndHit
	SetHitboxCanTakeDamage(entity, 1, false); //We know soft hitbox is always id 1
}

void MiddleHit(EntityID& entity, const int& index)
{
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	
	//Take knockback
	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	TransformComponent* transform = registry.GetComponent<TransformComponent>(entity);

	if ((!skelel && !bonel) || !cpc || !transform)
	{
		CancelTimedEvent(entity, index);
		return;
	}

	/*StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);
	if (cpc && transform)
	{
		float knockbackFactor = KNOCKBACK_FACTOR / (0.1f + transform->mass * GetTimedEventElapsedTime(entity, index));
		knockbackFactor *= knockbackFactor;
		transform->positionX += cpc->params.normal1X * GetDeltaTime() * knockbackFactor;
		transform->positionZ += cpc->params.normal1Z * GetDeltaTime() * knockbackFactor;
	}*/
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
	RedrawUI();//Bug fix redraw
}


void HazardBeginHit(EntityID& entity, const int& index)
{
	//Player sound of hurt entity
	EnemyComponent* enemy = registry.GetComponent<EnemyComponent>(entity);
	if (enemy != nullptr)
	{
		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		switch (enemy->type)
		{
		case EnemyType::hellhound:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Hellhound_Hurt, Channel_Base);
			}
			break;
		case EnemyType::eye:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Eye_Hurt, Channel_Base);
			}
			break;
		case EnemyType::skeleton:
			if (registry.GetComponent<StatComponent>(entity)->GetHealth() > 0)
			{
				sfx->Play(Skeleton_Hurt, Channel_Base);
			}
			break;
		}
	}

	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	HitInteraction(cpc->params.entity1, attackerStats, entity, stats);

	//PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);

	//// Calculate damage
	//float calculatedDamage = attackerStats->CalculateDamageDealt(stats);

	////Deal regular damage as well as on-hit damage from potential relics
	//stats->UpdateHealth(-calculatedDamage, player != nullptr);
	//auto funcVector = Relics::GetFunctionsOfType(Relics::FUNC_ON_DAMAGE_APPLY);
	//RelicInput::OnHitInput funcInput
	//{
	//	cpc->params.entity1,
	//	entity
	//};
	//for (uint32_t i = 0; i < funcVector.size(); ++i)
	//{
	//	funcVector[i](&funcInput);
	//}

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
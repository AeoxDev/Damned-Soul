#include "EventFunctions.h"
#include "Components.h"
#include "Registry.h"
#include "Relics/RelicFunctions.h"
#include "Relics\Utility\RelicFuncInputTypes.h" //Why isn't this included by RelicFunctions? Hermaaaaaaaaan
#include "DeltaTime.h"
#include "Levels/LevelHelper.h"
#include "UIRenderer.h"
#include "States\StateManager.h"
#include <cmath>
#include "CombatFunctions.h"

void BeginHit(EntityID& entity, const int& index)
{
	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);
	float preHealth = (float)stats->GetHealth();
	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	//Charged attack extravaganza
	TimedEventComponent* teComp = registry.GetComponent<TimedEventComponent>(entity);
	uint32_t condition = GetTimedEventCondition(teComp, index);

	Combat::HitInteraction(cpc->params.entity1, attackerStats, entity, stats, RelicInput::DMG::DAMAGE_TYPE_AND_SOURCE(condition));

	

	//Disable damage taken until EndHit if we're the player (enemy i-frames make faster attacks useless)
	auto player = registry.GetComponent<PlayerComponent>(entity);
	if(player)
		SetHitboxCanTakeDamage(entity, player->softHitboxID, false); //Only disables for like 0.3 seconds but it's better than nothing, you got reaction times

	//I want to play a sound that's just a straight-up *smack* for better hit feedback
}

void DashBeginHit(EntityID& entity, const int& index)
{
	//Get relevant components
	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);
	StatComponent* defenderStats = registry.GetComponent<StatComponent>(cpc->params.entity2);

	//Apply the damage
	Combat::DashHitInteraction(cpc->params.entity1, attackerStats, cpc->params.entity2, defenderStats);

	//Disable damage taken until EndHit
	auto player = registry.GetComponent<PlayerComponent>(entity);
	if(player)
		SetHitboxCanTakeDamage(entity, player->softHitboxID, false);
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
}

void EndHit(EntityID& entity, const int& index)
{
	//Enable damage taken again for the player
	auto player = registry.GetComponent<PlayerComponent>(entity);
	if (player)
		SetHitboxCanTakeDamage(entity, player->softHitboxID, true);

	//Make sure we're back to our regular color
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	//if (skelel)
	//	skelel->shared.colorAdditiveRed = 0.0f;
	//if (bonel)
	//	bonel->shared.colorAdditiveRed = 0.0f;
	RedrawUI();//Bug fix redraw
}


void HazardBeginHit(EntityID& entity, const int& index)
{
	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	
	Combat::HitInteraction(cpc->params.entity1, attackerStats, entity, stats);

	//Become red
	if (skelel)
		skelel->shared.bcaR_temp = 1.0f;
	if (bonel)
		bonel->shared.bcaR_temp = 1.0f;
}
void HazardEndHit(EntityID& entity, const int& index)
{
	//Make sure we're back to our regular color
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	//if (skelel)
	//	skelel->shared.colorAdditiveRed = 0.0f;
	//if (bonel)
	//	bonel->shared.colorAdditiveRed = 0.0f;
}

void StaticHazardDamage(EntityID& entity, const int& index)
{
	int condition = GetTimedEventCondition(entity, index);
	StatComponent* stat = registry.GetComponent<StatComponent>(entity);
	int cameraShake = 0;
	int color = 0;
	switch (condition)
	{
	case HAZARD_LAVA:
		//if (duck relic, skip)
		if (stat->hazardModifier == 0.0f)
		{
			return;
		}
		if (entity.index == stateManager.player.index)
		{
			cameraShake = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, ShakeCamera, HAZARD_LAVA_UPDATE_TIME, ResetCameraOffset, 0, 1);
		}
		stat->ApplyDamage(HAZARD_LAVA_DAMAGE * stat->hazardModifier, entity.index == stateManager.player.index);
		color = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, LavaBlinkColor, HAZARD_LAVA_UPDATE_TIME, /*ResetColor*/nullptr); //No special condition for now
		break;
	default:
		break;
	}
}

void LavaBlinkColor(EntityID& entity, const int& index)
{
	//Flash color red repeatedly
	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);
	float frequency = 16.0f; //Higher frequency = faster flashing lights
	float cosineWave = std::cosf(GetTimedEventElapsedTime(entity, index) * frequency) * std::cosf(GetTimedEventElapsedTime(entity, index) * frequency);
	if (skelel)
	{
		skelel->shared.bcaR_temp *= cosineWave;
		skelel->shared.bcaR_temp += 0.2f * cosineWave;
	}
		
	if (bonel)
	{
		bonel->shared.bcaR_temp *= cosineWave;
		bonel->shared.bcaG_temp += 0.2f * cosineWave;
	}
}

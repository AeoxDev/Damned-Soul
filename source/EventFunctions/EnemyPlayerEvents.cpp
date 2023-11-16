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
//#include <cmath> //sin

#define KNOCKBACK_FACTOR 0.3f

void BeginHit(EntityID& entity, const int& index)
{
	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	//Charged attack extravaganza
	TimedEventComponent* teComp = registry.GetComponent<TimedEventComponent>(entity);
	uint32_t condition = GetTimedEventCondition(teComp, index);
	
	Combat::HitInteraction(cpc->params.entity1, attackerStats, entity, stats, condition == CONDITION_CHARGE);

	//Disable damage taken until EndHit if we're the player (enemy i-frames make faster attacks useless)
	if(registry.GetComponent<PlayerComponent>(entity) != nullptr)
		SetHitboxCanTakeDamage(entity, 1, false); //We know soft hitbox is always id 1

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
}

void EndHit(EntityID& entity, const int& index)
{
	//Enable damage taken again for the player
	if (registry.GetComponent<PlayerComponent>(entity) != nullptr)
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
	else
	{
		PlayerComponent* player = registry.GetComponent<PlayerComponent>(entity);
		if (player != nullptr)
		{
			registry.GetComponent<SoundComponent>(entity)->Play(Player_Hurt, Channel_Base);
		}
	}

	//Get relevant components
	StatComponent* stats = registry.GetComponent<StatComponent>(entity);

	CollisionParamsComponent* cpc = registry.GetComponent<CollisionParamsComponent>(entity);
	StatComponent* attackerStats = registry.GetComponent<StatComponent>(cpc->params.entity1);

	ModelSkeletonComponent* skelel = registry.GetComponent<ModelSkeletonComponent>(entity);
	ModelBonelessComponent* bonel = registry.GetComponent<ModelBonelessComponent>(entity);

	Combat::HitInteraction(cpc->params.entity1, attackerStats, entity, stats);

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
		color = AddTimedEventComponentStartContinuousEnd(entity, 0.0f, nullptr, LavaBlinkColor, HAZARD_LAVA_UPDATE_TIME, ResetColor); //No special condition for now
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
		skelel->colorAdditiveRed = cosineWave;
		skelel->colorAdditiveGreen = 0.2f * cosineWave;
	}
		
	if (bonel)
	{
		bonel->colorAdditiveRed = cosineWave;
		bonel->colorAdditiveGreen = 0.2f * cosineWave;
	}
}

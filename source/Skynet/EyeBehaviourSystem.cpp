#include "Systems\Systems.h"
#include "EntityFramework.h"
#include "Registry.h"
#include "DeltaTime.h"
#include "Skynet\BehaviourHelper.h"
#include "UI/UIRenderer.h"
#include "EventFunctions.h"
#include "States\StateManager.h"

#include <algorithm>
#include <random>
#include <iostream>

#define OBSTACLE_RANGE 5

void RetreatBehaviour(PlayerComponent* pc, TransformComponent* ptc, EyeBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats, AnimationComponent* enemyAnim, ObstacleMap* valueGrid)
{
	// Regular walk
	if (enemyAnim->aAnim != ANIMATION_WALK || (enemyAnim->aAnim == ANIMATION_WALK && enemyAnim->aAnimIdx != 0))
	{
		enemyAnim->aAnim = ANIMATION_WALK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

	//using charge timer as a safetynet 
	ec->chargeTimer += GetDeltaTime();
	if (!ec->retreating)
	{
		ec->retreating = true;
		float minRange = 20.0f;
		float maxRange = 30.0f;

		TransformComponent newTile = FindRetreatTile(valueGrid, etc, 20.f, 30.f);

		float dirToPlayerX = ptc->positionX - etc->positionX;
		float dirToPlayerZ = ptc->positionZ - etc->positionZ;
		Normalize(dirToPlayerX, dirToPlayerZ);

		float dirToNewTileX = newTile.positionX - etc->positionX;
		float dirToNewTileZ = newTile.positionZ - etc->positionZ;
		Normalize(dirToNewTileX, dirToNewTileZ);

		float dotX = dirToNewTileX * dirToPlayerX;
		float dotZ = dirToNewTileZ * dirToPlayerZ;
		int iterations = 0;
		while ((dotX + dotZ > 0.0f) && iterations < 1000)
		{
			newTile = FindRetreatTile(valueGrid, etc, 20.f, 30.f);

			float dirToNewTileX = newTile.positionX - etc->positionX;
			float dirToNewTileZ = newTile.positionZ - etc->positionZ;
			Normalize(dirToNewTileX, dirToNewTileZ);

			dotX = dirToNewTileX * dirToPlayerX;
			dotZ = dirToNewTileZ * dirToPlayerZ;
			iterations++;
		}

		ec->targetX = newTile.positionX;
		ec->targetZ = newTile.positionZ;

		//adjust facing based on obstacle avoidance
		ec->goalDirectionX = dirToNewTileX + ec->correcitonDirX;
		ec->goalDirectionZ = dirToNewTileZ + ec->correcitonDirX;
		
		SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);

		etc->positionX += etc->facingX * enemyStats->GetSpeed() * GetDeltaTime();
		etc->positionZ += etc->facingZ * enemyStats->GetSpeed() * GetDeltaTime();
	}
	else if(ec->chargeTimer < 1.5f)
	{
		auto dist = Calculate2dDistance(etc->positionX, etc->positionZ, ec->targetX, ec->targetZ);
		if (dist > 1.0f)
		{

			SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
			etc->positionX += etc->facingX * enemyStats->GetSpeed() * GetDeltaTime();
			etc->positionZ += etc->facingZ * enemyStats->GetSpeed() * GetDeltaTime();
		}
		else
		{
			ec->retreating = false;
			ec->chargeTimer = 0.0f;
		}
	}
	else 
	{
		ec->retreating = false;
		ec->chargeTimer = 0.0f;
	}
}

bool CombatBehaviour(EntityID entity, PlayerComponent*& pc, TransformComponent*& ptc, EyeBehaviour*& ec, TransformComponent*& etc, StatComponent*& enemyStats, StatComponent*& playerStats, AnimationComponent* enemyAnim)
{
	//if you just attacked go back to circle behaviour
	if (ec->attackTimer < enemyStats->GetAttackSpeed())
	{
		ec->attackTimer += GetDeltaTime();
		return false;
	}
	//rotate eye in order to shoot at the player
	else if (ec->aimTimer < ec->aimDuration)
	{
		if (ec->aimTimer == 0.0f)
		{
			ec->shooting = true;
			AddTimedEventComponentStartContinous(entity, 0.0f, nullptr, ec->aimDuration - 0.2f, EnemyAttackFlash);

			if (enemyAnim->aAnim != ANIMATION_ATTACK || (enemyAnim->aAnim == ANIMATION_ATTACK && enemyAnim->aAnimIdx != 1))
			{
				enemyAnim->aAnim = ANIMATION_ATTACK;
				enemyAnim->aAnimIdx = 1;
				enemyAnim->aAnimTime = 0.0f;
				enemyAnim->aAnimTimeFactor = 1.0f / ec->aimDuration;
			}
		}

		ec->goalDirectionX = ptc->positionX - etc->positionX;
		ec->goalDirectionZ = ptc->positionZ - etc->positionZ;

		Normalize(ec->goalDirectionX, ec->goalDirectionZ);


		SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
		
		ec->aimTimer += GetDeltaTime();
		ec->attackTimer += GetDeltaTime();
		return true;
	}
	else // yes, we can indeed attack. 
	{
		ec->attackTimer = 0;
		ec->aimTimer = 0;
		ec->specialCounter++; //increase the special counter for special attack
		ec->attackStunTimer = 0;
		ec->shooting = false;

		enemyAnim->aAnim = ANIMATION_IDLE;
		enemyAnim->aAnimIdx = 1;
		enemyAnim->aAnimTime = 0.0f;
		enemyAnim->aAnimTimeFactor = 1.0f;

		//set direction for attack
		float dx = ptc->positionX - etc->positionX;
		float dz = ptc->positionZ - etc->positionZ;
		
		Normalize(dx, dz);

		CreateProjectile(entity, dx, dz, eye);

		SoundComponent* sfx = registry.GetComponent<SoundComponent>(entity);
		if(sfx != nullptr) sfx->Play(Eye_Shoot, Channel_Base);

		return true;
	}
}

void CircleBehaviour(PlayerComponent* pc, TransformComponent* ptc, EyeBehaviour* ec, TransformComponent* etc, StatComponent* enemyStats, StatComponent* playerStats, AnimationComponent* enemyAnim)
{
	if (enemyAnim->aAnim != ANIMATION_WALK || (enemyAnim->aAnim == ANIMATION_WALK && enemyAnim->aAnimIdx != 0))
	{
		enemyAnim->aAnim = ANIMATION_WALK;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

	float magnitude = 0.f;
	float dirX = 0.f;
	float dirZ = 0.f;

	if (ec->clockwiseCircle) //clockwise
	{
		dirX = -ec->goalDirectionZ;
		dirZ = ec->goalDirectionX;
	}
	else // counter clockwise
	{
		dirX = ec->goalDirectionZ;
		dirZ = -ec->goalDirectionX;
	}

	dirX += ec->correcitonDirX;
	dirZ += ec->correcitonDirZ;

	Normalize(dirX, dirZ);

	etc->positionX += dirX * enemyStats->GetSpeed() * GetDeltaTime();
	etc->positionZ += dirZ * enemyStats->GetSpeed() * GetDeltaTime();
	ec->goalDirectionX = ptc->positionX - etc->positionX;
	ec->goalDirectionZ = ptc->positionZ - etc->positionZ;
	
	/*ec->goalDirectionX += ec->correcitonDirX;
	ec->goalDirectionZ += ec->correcitonDirZ;*/
	Normalize(ec->goalDirectionX, ec->goalDirectionZ);

	SmoothRotation(etc, ec->goalDirectionX, ec->goalDirectionZ, 30.f);
}

void IdleBehaviour(EntityID& enemy, PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats, AnimationComponent* enemyAnim)
{
	if (enemyAnim->aAnim != ANIMATION_IDLE || (enemyAnim->aAnim == ANIMATION_IDLE && enemyAnim->aAnimIdx != 0))
	{
		enemyAnim->aAnim = ANIMATION_IDLE;
		enemyAnim->aAnimIdx = 0;
		enemyAnim->aAnimTime = 0.0f;
	}

	if (eyeComponent->timeCounter >= eyeComponent->updateInterval)
	{
		eyeComponent->timeCounter = 0.f;
		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [-1.0, 1.0]
		std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
		float randomX = distribution(gen);
		float randomZ = distribution(gen);
		eyeComponent->goalDirectionX = randomX;
		eyeComponent->goalDirectionZ = randomZ;
		std::uniform_real_distribution<float> randomInterval(1.0f, 1.5f);
		eyeComponent->updateInterval = randomInterval(gen);
	}

	//adjust facing based on obstacle avoidance
	eyeComponent->goalDirectionX += eyeComponent->correcitonDirX;
	eyeComponent->goalDirectionZ += eyeComponent->correcitonDirZ;

	SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ, 5.0f);


	eyeTransformComponent->positionX += eyeTransformComponent->facingX * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
	eyeTransformComponent->positionZ += eyeTransformComponent->facingZ * enemyStats->GetSpeed() * 0.5f * GetDeltaTime();
	eyeComponent->timeCounter += GetDeltaTime();
}

void ChargeBehaviour(PlayerComponent* playerComponent, TransformComponent* playerTransformCompenent, EyeBehaviour* eyeComponent, TransformComponent* eyeTransformComponent, StatComponent* enemyStats, StatComponent* playerStats, HitboxComponent* enemyHitbox, EntityID eID, EnemyComponent* enemComp, AnimationComponent* enemyAnim)
{
	if (!eyeComponent->charging)
	{
		//reset value and calculate new breakpoint
		eyeComponent->attackTimer = 0;
		eyeComponent->specialCounter = 0;
		eyeComponent->charging = true;

		std::random_device rd;
		std::mt19937 gen(rd());
		// Define a uniform distribution for the range [2.0, 5.0]
		std::uniform_real_distribution<float> distribution(2.0f, 5.0f);
		eyeComponent->specialBreakpoint = (int)distribution(gen);

		enemyStats->SetKnockbackMultiplier(1.5f);
		//direction from the enemy towards the player
		float dirX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
		float dirZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

		Normalize(dirX, dirZ);

		//target is the stopping point of the charge, a set distance behind the players position
		eyeComponent->targetX = playerTransformCompenent->positionX + dirX * 10.0f;
		eyeComponent->targetZ = playerTransformCompenent->positionZ + dirZ * 10.0f;

		eyeComponent->chargeDirX = dirX; // charge direction is only set once per charge and will not change
		eyeComponent->chargeDirZ = dirZ; 

		//change what direction the eye is circling after each dash
		//(eyeComponent->clockwiseCircle == true) ? eyeComponent->clockwiseCircle = false : eyeComponent->clockwiseCircle = true;

		SmoothRotation(eyeTransformComponent, eyeComponent->chargeDirX, eyeComponent->chargeDirZ, 40.0f);
		AddTimedEventComponentStartContinuousEnd(eID, 0.0f, nullptr, ChargeColorFlash, eyeComponent->aimDuration - 0.2f, ResetColor);
	}
	else if (eyeComponent->aimTimer < eyeComponent->aimDuration)
	{
		eyeComponent->goalDirectionX = playerTransformCompenent->positionX - eyeTransformComponent->positionX;
		eyeComponent->goalDirectionZ = playerTransformCompenent->positionZ - eyeTransformComponent->positionZ;

		Normalize(eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ);

		SmoothRotation(eyeTransformComponent, eyeComponent->goalDirectionX, eyeComponent->goalDirectionZ, 30.f);
		eyeComponent->aimTimer += GetDeltaTime();
		eyeComponent->attackTimer += GetDeltaTime();
	}
	else 
	{
		if (enemyAnim->aAnim != ANIMATION_ATTACK || (enemyAnim->aAnim == ANIMATION_ATTACK && enemyAnim->aAnimIdx != 0))
		{
			enemyAnim->aAnim = ANIMATION_ATTACK;
			enemyAnim->aAnimIdx = 0;
			enemyAnim->aAnimTime = 0.0f;

			SetHitboxActive(eID, enemComp->attackHitBoxID, true);
			SetHitboxCanDealDamage(eID, enemComp->attackHitBoxID, true);
			SetHitboxIsEnemy(eID, 0, false);
			SetHitboxIsEnemy(eID, 1, false);
		}
		
		//calculate the current direction towards player
		float dirX = (eyeComponent->targetX - eyeTransformComponent->positionX);
		float dirZ = (eyeComponent->targetZ - eyeTransformComponent->positionZ);

		Normalize(dirX, dirZ);

		//scalar between the current direction and the original chagre direction
		float scalar = dirX * eyeComponent->chargeDirX + dirZ * eyeComponent->chargeDirZ;

		//If charging scalar point direction > 0.0, charge
		if (scalar > 0 && eyeComponent->chargeTimer < 1.0f)
		{
			eyeComponent->chargeTimer += GetDeltaTime();
			SmoothRotation(eyeTransformComponent, eyeComponent->chargeDirX, eyeComponent->chargeDirZ, 30.0f);

			eyeTransformComponent->positionX += eyeComponent->chargeDirX * enemyStats->GetSpeed() * 6.f * GetDeltaTime();
			eyeTransformComponent->positionZ += eyeComponent->chargeDirZ * enemyStats->GetSpeed() * 6.f * GetDeltaTime();
			
		}
		else //else charge is finished
		{
			enemyStats->SetKnockbackMultiplier(1.0f);
			////reenable hitboxes
			SetHitboxIsEnemy(eID, 0, true);
			SetHitboxIsEnemy(eID, 1, true);
			SetHitboxActive(eID, enemComp->attackHitBoxID, false);
			SetHitboxCanDealDamage(eID, enemComp->attackHitBoxID, false);

			//reset values
			eyeComponent->charging = false;
			eyeComponent->chargeAttackSoundPlaying = false;
			eyeComponent->attackTimer = 0;
			eyeComponent->attackStunTimer = 0;
			eyeComponent->dealtDamage = false;
			eyeComponent->aimTimer = 0.0f;

			enemyAnim->aAnimTime = 0.0f;
			enemyAnim->aAnim = ANIMATION_IDLE;
			enemyAnim->aAnimIdx = 1;
		}
	}
}

void GetNeighbours(GridPosition currentPos, GridPosition startPos, ML_Vector<GridPosition>& openList, ML_Vector<GridPosition>& closedList, Coordinate2D direction)
{
	GridPosition p1 = currentPos;
	GridPosition p2 = currentPos;
	GridPosition p3 = currentPos;
	
	if (direction.x == 0)
	{
		//create new points, left or right in the grid based on direction
		p1.x += 1;
		p2.x -= 1;
		p3.z += direction.z;
		
		
	}
	else // direction.z == 0
	{
		//create new points, up or down in the grid based on direction
		p1.z += 1;
		p2.z -= 1;
		p3.x += direction.x;
	}

	bool addP1 = true;
	bool addP2 = true;
	bool addP3 = true;

	//check distance
	if (sqrt((p1.x - startPos.x) * (p1.x - startPos.x) + (p1.z - startPos.z) * (p1.z - startPos.z)) > OBSTACLE_RANGE)
		addP1 = false;
	if (sqrt((p2.x - startPos.x) * (p2.x - startPos.x) + (p2.z - startPos.z) * (p2.z - startPos.z)) > OBSTACLE_RANGE)
		addP2 = false;
	if (sqrt((p3.x - startPos.x) * (p3.x - startPos.x) + (p3.z - startPos.z) * (p3.z - startPos.z)) > OBSTACLE_RANGE)
		addP3 = false;

	//are they in the closedlist?
	for (unsigned int i = 0; i < closedList.size(); ++i)
	{
		if (addP1)
		{
			if (closedList[i].x == p1.x && closedList[i].z == p1.z)
			{
				addP1 = false;
			}
		}
		if (addP2)
		{
			if (closedList[i].x == p2.x && closedList[i].z == p2.z)
			{
				addP2 = false;
			}
		}
		if (addP3)
		{
			if (closedList[i].x == p3.x && closedList[i].z == p3.z)
			{
				addP3 = false;
			}
		}
	}

	//add the legal tiles to openlist
	if (addP1)
	{
		openList.push_back(p1);
		closedList.push_back(p1);
	}
	if (addP2)
	{
		openList.push_back(p2);
		closedList.push_back(p2);
	}
	if (addP3)
	{
		openList.push_back(p3);
		closedList.push_back(p3);
	}
}

void ObstacleAvoidance(EyeBehaviour* ec, TransformComponent* etc, ObstacleMap* valueGrid)
{
	ec->correcitonDirX = 0.0f;
	ec->correcitonDirZ = 0.0f;
	
	if (etc->lastPositionX == 0 && etc->lastPositionZ == 0)
		return;

	//float eyeMovementX = etc->positionX - etc->lastPositionX;
	//float eyeMovementZ = etc->positionZ - etc->lastPositionZ;

	float eyeMovementX = etc->facingX;
	float eyeMovementZ = etc->facingZ;

	if (eyeMovementX == 0 && eyeMovementZ == 0)
		return;

	GeometryIndependentComponent* GIcomponent = registry.GetComponent<GeometryIndependentComponent>(stateManager.stage);

	float dirX = eyeMovementX;
	float dirZ = eyeMovementZ;

	Coordinate2D direction = { 0.0f, 0.0f };

	float magX = sqrt(dirX * dirX);
	float magZ = sqrt(dirZ * dirZ);

	if (magX > magZ)
	{
		if (dirX > 0.0f)
		{
			direction.x = 1.0f;
		}
		else 
		{
			direction.x = -1.0f;
		}
	}
	else
	{
		if (dirZ > 0.0f)
		{
			direction.z = -1.0f;
		}
		else
		{
			direction.z = 1.0f;
		}
	}

	GridPosition startPos = PositionOnGrid(GIcomponent, etc, GI_TEXTURE_DIMENSIONS_FOR_OBSTACLEAVOIDANCE);
	
	ML_Vector<GridPosition> openList;
	ML_Vector<GridPosition> closedList;

	closedList.push_back(startPos);

	GetNeighbours(startPos, startPos, openList, closedList, direction);

	Coordinate2D finalDirection = { 0.0f ,0.0f };
	while (openList.size() > 0)
	{
		//are you wall?
		GridPosition currentTile = openList[0];
		openList.erase(0);

		float cost = valueGrid->cost[currentTile.x][currentTile.z];

		if ( cost >= 5000) //is it wall or an enemy
		{
			//apply force

			Coordinate2D tileWorldPos = GridOnPosition(currentTile, GIcomponent, GI_TEXTURE_DIMENSIONS_FOR_OBSTACLEAVOIDANCE);
			float pushbackX = etc->positionX - tileWorldPos.x;
			float pushbackZ = etc->positionZ - tileWorldPos.z;
			
			float distance = Calculate2dDistance((float)currentTile.x, (float)currentTile.z, (float)startPos.x, (float)startPos.z);

			float multiplier = (float)(1 - pow(distance / OBSTACLE_RANGE, 2));

			if (cost < 10000) //if tile is an enemy
				multiplier *= 0.5f;

			finalDirection.x += pushbackX * multiplier;
			finalDirection.z += pushbackZ * multiplier;
		}
		else 
		{
			GetNeighbours(currentTile, startPos, openList, closedList, direction);
		}
	}
	Normalize(finalDirection.x, finalDirection.z);

	ec->correcitonDirX = finalDirection.x;
  	ec->correcitonDirZ = finalDirection.z;

	if (finalDirection.x > 0.0)
		ec->clockwiseCircle = true;
	else if(finalDirection.x < 0.0)
		ec->clockwiseCircle = false;
}

bool EyeBehaviourSystem::Update()
{
	//First find the skynet component
	PlayerComponent* playerComponent = nullptr;
	TransformComponent* playerTransformCompenent = nullptr;
	EyeBehaviour* eyeComponent = nullptr;
	TransformComponent* eyeTransformComponent = nullptr;
	HitboxComponent* enemyHitbox = nullptr;
	StatComponent* enemyStats = nullptr;
	StatComponent* playerStats = nullptr;
	EnemyComponent* enemComp = nullptr;
	DebuffComponent* debuff = nullptr;

	bool hasUpdatedMap = false;
	ObstacleMap* valueGrid = (ObstacleMap*)malloc(sizeof(ObstacleMap));
	
	//Find available entity
	for (auto enemyEntity : View<EyeBehaviour, TransformComponent, HitboxComponent, EnemyComponent>(registry))
	{
		SetLightColor(enemyEntity, 0.3f, 0.3f, 0.3f);
		eyeComponent = registry.GetComponent<EyeBehaviour>(enemyEntity);
		eyeTransformComponent = registry.GetComponent<TransformComponent>(enemyEntity);
		enemyStats = registry.GetComponent<StatComponent>(enemyEntity);
		enemyHitbox = registry.GetComponent<HitboxComponent>(enemyEntity);
		enemComp = registry.GetComponent<EnemyComponent>(enemyEntity);
		AnimationComponent* enemyAnim = registry.GetComponent<AnimationComponent>(enemyEntity);

		debuff = registry.GetComponent<DebuffComponent>(enemyEntity);
		if (debuff && debuff->m_frozen)
		{
			TransformDecelerate(enemyEntity);//Always decelerate
			continue; // frozen, won't do behavior stuff
		}


		//Find a player to kill.
		if (enemComp->lastPlayer.index == -1)
		{
			for (auto playerEntity : View<PlayerComponent, TransformComponent>(registry))
			{
				if (enemyEntity.index == playerEntity.index)
				{
					continue;
				}

				playerComponent = registry.GetComponent<PlayerComponent>(playerEntity);
				playerTransformCompenent = registry.GetComponent<TransformComponent>(playerEntity);
				playerStats = registry.GetComponent< StatComponent>(playerEntity);
				enemComp->lastPlayer = playerEntity;
				if (rand() % 2)
				{
					break;
				}
			}
		}
		else
		{
			playerComponent = registry.GetComponent<PlayerComponent>(enemComp->lastPlayer);
			playerTransformCompenent = registry.GetComponent<TransformComponent>(enemComp->lastPlayer);
			playerStats = registry.GetComponent< StatComponent>(enemComp->lastPlayer);
			if (playerComponent == nullptr)
			{
				enemComp->lastPlayer.index = -1;
			}
		}
		

		if (enemyStats->GetHealth() > 0 && eyeComponent != nullptr && playerTransformCompenent != nullptr && enemyHitbox != nullptr && enemComp != nullptr)// check if enemy is alive
		{
			float distance = Calculate2dDistance(eyeTransformComponent->positionX, eyeTransformComponent->positionZ, playerTransformCompenent->positionX, playerTransformCompenent->positionZ);
			
			//distance = 10000;

			if (eyeComponent->attackStunTimer <= eyeComponent->attackStunDuration)
			{
				if (enemyAnim->aAnim != ANIMATION_IDLE || (enemyAnim->aAnim == ANIMATION_IDLE && enemyAnim->aAnimIdx != 1))
				{
					enemyAnim->aAnim = ANIMATION_IDLE;
					enemyAnim->aAnimIdx = 1;
					enemyAnim->aAnimTime = 0.0f;
				}
			}
			else if ((distance < eyeComponent->range/6.0f || eyeComponent->retreating) && !eyeComponent->charging && !eyeComponent->shooting) // Retreat to safe distance if not charging
			{
				if (hasUpdatedMap == false)
				{
					hasUpdatedMap = true;
					CalculateGlobalMapValuesEye(valueGrid);
				}
				RetreatBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyAnim, valueGrid);
			}
			else if (eyeComponent->charging || (eyeComponent->specialCounter >= eyeComponent->specialBreakpoint && eyeComponent->attackTimer >= enemyStats->GetAttackSpeed())) //if special is ready or is currently doing special
			{
				//CHAAAAARGE
				if (!eyeComponent->chargeAttackSoundPlaying)
				{
					eyeComponent->chargeAttackSoundPlaying = true;
					SoundComponent* sfx = registry.GetComponent<SoundComponent>(enemyEntity);
					sfx->Play(Eye_Attack, Channel_Base);
					eyeComponent->chargeTimer = 0.0f;
				}
				ChargeBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyHitbox, enemyEntity, enemComp, enemyAnim);

			}
			else if (eyeComponent->shooting || distance <= eyeComponent->range/1.5f) // circle player & attack when possible
			{
				if (!CombatBehaviour(enemyEntity, playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyAnim))
					CircleBehaviour(playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, playerStats, enemyAnim);
			}
			else // idle
			{
				enemComp->lastPlayer.index = -1;//Search for a new player to hit.
				IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyAnim);
			}
			
			if (!eyeComponent->charging)
			{
				if (hasUpdatedMap == false)
				{
					hasUpdatedMap = true;
					CalculateGlobalMapValuesEye(valueGrid);
				}
				ObstacleAvoidance(eyeComponent, eyeTransformComponent, valueGrid);
			}
		}
		//Idle if there are no players on screen.
		else if (enemyStats->GetHealth() > 0.0f)
		{
			enemComp->lastPlayer.index = -1;//Search for a new player to hit.
			IdleBehaviour(enemyEntity, playerComponent, playerTransformCompenent, eyeComponent, eyeTransformComponent, enemyStats, enemyAnim);
		}

		eyeComponent->attackStunTimer += GetDeltaTime();
		enemyAnim->aAnimTime += GetDeltaTime() * enemyAnim->aAnimTimeFactor;
		ANIM_BRANCHLESS(enemyAnim);
		TransformDecelerate(enemyEntity);
	}

	free(valueGrid);

	return true;
}